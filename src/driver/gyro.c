#include "gyro.h"
#include "board.h"
#include "pcbuffer.h"
#include "gpio.h"
#include <string.h>
#include <stdlib.h>

gyro_t gyro;


spi_transaction_t write_transaction;
uint8_t write_tx_buf[2], write_rx_buf[2];

const uint32_t drift_times[3] = {
    GYRO_X_DRIFT_T, GYRO_Y_DRIFT_T, GYRO_Z_DRIFT_T
};

void write_done_discard(const uint8_t *buffer, size_t len)
{ (void) buffer; (void) len; }

/*
 * Perform a blocking register write.
 */
void gyro_write_block(uint8_t reg, uint8_t data)
{
    write_tx_buf[0] = reg; write_tx_buf[1] = data;
    spi_start_transaction(&spi1_state, &write_transaction);
    spi_wait_on_transaction(&write_transaction);
}

/*
 * Perform a blocking register read.
 */
uint8_t gyro_read_block(uint8_t reg)
{
    write_tx_buf[0] = 0x80 | reg; write_tx_buf[1] = 0x00;
    spi_start_transaction(&spi1_state, &write_transaction);
    spi_wait_on_transaction(&write_transaction);
    return write_rx_buf[1];
}

/*
 * Verify that an internal register is at a specific value.
 */
int gyro_validate_register(uint8_t address, uint8_t expected)
{
    uint8_t result = gyro_read_block(address);
    if (result != expected)
    {
        printf("gyro 0x%x: 0x%x != 0x%x", address, result, expected);
        return -1;
    }
    return 0;
}

float raw_to_float(const uint8_t reading[2])
{
    float temp;
    int16_t raw;
    raw = reading[0] << 8 | reading[1];
    temp = (float) raw;
    temp *= GYRO_SENSITIVITY;
    temp /= GYRO_DATA_RATE;
    return temp;
}

void calibration_round_done(const uint8_t *buffer, size_t len)
{
    float temp;

    __disable_irq();

    gyro.status = buffer[1];
    for (size_t i = 2; i < len; i += GYRO_SAMPLE_SIZE)
    {
        /* msb comes first per 16-bit value */
        gyro.calib_offset[0] += raw_to_float(&buffer[i]);
        gyro.calib_offset[1] += raw_to_float(&buffer[i + 2]);
        gyro.calib_offset[2] += raw_to_float(&buffer[i + 4]);
        gyro.calib_samples++;
    }

    /* advance to normal sampling if calibration is complete, otherwise
     * continue with calibration */
    gyro.state = GYRO_NOT_CALIB;
    if (gyro.calib_samples >= GYRO_CALIB_SAMPLES)
    {
        temp = (float) gyro.calib_samples;
        gyro.calib_offset[0] /= temp;
        gyro.calib_offset[1] /= temp;
        gyro.calib_offset[2] /= temp;
        gyro.state = GYRO_BUFFERING;
    }

    __enable_irq();
}

void sample_round_done(const uint8_t *buffer, size_t len)
{
    size_t i;
    uint32_t sample_time;

    __disable_irq();

    gyro.status = buffer[1];

    /* accumulate all of the samples */
    for (i = 2; i < len; i += GYRO_SAMPLE_SIZE)
    {
        gyro.accum[0] += raw_to_float(&buffer[i]);
        gyro.accum[1] += raw_to_float(&buffer[i + 2]);
        gyro.accum[2] += raw_to_float(&buffer[i + 4]);
        gyro.samples++;
    }

    /* apply manually calculated drift offsets if necessary */
    sample_time = ticks - gyro.samples_start_t;
    for (i = 0; i < 3; i++)
    {
        if (sample_time / drift_times[i] > gyro.drifts_applied[i])
        {
            gyro.drifts_applied[i]++;
            gyro.accum[i] += GYRO_DRIFT_AMT;
        }
    }

    /* can't generate floating-point instructions to write a value to an
     * address that's not word-aligned, so use memcpy */
    memcpy(manifest.channels[0].data, &gyro.accum[0], sizeof(float));
    memcpy(manifest.channels[1].data, &gyro.accum[1], sizeof(float));
    memcpy(manifest.channels[2].data, &gyro.accum[2], sizeof(float));

    /* inform handler that we can start new samples */
    gyro.state = GYRO_BUFFERING;

    gyro.new_data = true;

    __enable_irq();
}

void service_gyro(gyro_t *gyro)
{
    size_t new_write, new_read;
    if (gpio_readPin(GPIOC, 0))
    {
        /* if we detected an overflow, clear the buffer */
        if (gyro->status & 0x80)
        {
            new_write = GYRO_BUFFER_SIZE;
            new_read = GYRO_BUFFER_SIZE;
        }
        /* read the normal watermark amount */
        else
        {
            new_write = 2 + (GYRO_SAMPLE_SIZE * GYRO_FIFO_WATERMARK);
            new_read =  2 + (GYRO_SAMPLE_SIZE * GYRO_FIFO_WATERMARK);
        }
        switch (gyro->state)
        {
            /* samples are ready and we're still calibrating an offset */
            case GYRO_NOT_CALIB:
                gyro->sample.to_write = new_write;
                gyro->sample.to_read  = new_read;
                gyro->sample.callback = calibration_round_done;
                spi_start_transaction(gyro->spi, &gyro->sample);
                break;

            /* samples are ready and we're taking samples normally */
            case GYRO_BUFFERING:
                if (gyro->samples == 0) gyro->samples_start_t = ticks;
                gyro->sample.to_write = new_write;
                gyro->sample.to_read  = new_read;
                gyro->sample.callback = sample_round_done;
                spi_start_transaction(gyro->spi, &gyro->sample);
                break;
            default: return;
        }
    }
}

void gyro_init(gyro_t *gyro)
{
    memset(gyro, 0, sizeof(gyro_t));
    gyro->spi             = &spi1_state;
    gyro->calib_samples   = 0;
    gyro->samples_start_t = 0;
    gyro->new_data = false;
    memset(gyro->write_buffer, 0, GYRO_BUFFER_SIZE);
    memset(gyro->read_buffer,  0, GYRO_BUFFER_SIZE);
    spi_init_transaction(&gyro->sample, gyro->write_buffer, gyro->read_buffer,
                         0, 0, command_set_spi1_cs, command_reset_spi1_cs,
                         write_done_discard);

    /* set the read bit for the first byte shifted out, keep the address at
     * zero so we always read the status register first */
    gyro->write_buffer[0] = 0x80;
}

/*
 * Perform initializations for the sensor.
 */
int gyro_config(void)
{
    uint8_t control;

    /* initialize the gyro struct */
    gyro_init(&gyro);

    /* initialize the global writing transaction */
    spi_init_transaction(&write_transaction, write_tx_buf, 
                         write_rx_buf, 2, 2,
                         command_set_spi1_cs, command_reset_spi1_cs, 
                         write_done_discard);

    /* bring sensor out of a hardware reset state */
    gpio_setPin(GPIOC, 2); delay(10);

    /* verify connectivity by reading the 'who_am_i' register */
    if (gyro_validate_register(WHO_AM_I_21002, GYRO_WHOAMI_VAL))
        return -1;

    /* Reset the device and put it in standby mode. */
    control = SOFT_RESET_21002 | MODE_STANDBY_21002;
    gyro_write_block(CTRL_REG1_21002, control);
    delay(10);

    /*
     * Control Register 0:
     *
     * Set the acquisition sensitivity fot the sensor, plus low-pass and
     * high-pass filter parameters.
     *
     * These configurations should be experimented with.
     */
    control = FS_250_DPS_21002 | LPF_BW_HIGH_21002 |
              HPF_BW_ODR_DIV400 | HPF_EN_21002;
    gyro_write_block(CTRL_REG0_21002, control);
    if (gyro_validate_register(CTRL_REG0_21002, control))
        return -1;

    /*
     * Control Register 1:
     *
     * Set the sample rate for the sensor (800 Hz, maximum) and put
     * the device into 'ready' mode.
     */
    control = ODR_800_HZ_21002;
    gyro_write_block(CTRL_REG1_21002, control);
    if (gyro_validate_register(CTRL_REG1_21002, control))
        return -1;

    /*
     * Control Register 2:
     *
     * Route the FIFO interrupt to INT1 and enable it as active-high,
     * push-pull.
     */
    control = INT1_CFG_FIFO_21002 | INT_EN_FIFO_21002 |
              IPOL_ACTIVE_HIGH_21002 | PP_OD_PUSH_PULL_21002;
    gyro_write_block(CTRL_REG2_21002, control);
    if (gyro_validate_register(CTRL_REG2_21002, control))
        return -1;

    /*
     * Control Register 3:
     *
     * Set WRAPTOONE to allow a continuous read of multiple samples,
     * optionally set FS_DOUBLE to experiment with double-sensitivity
     * readings.
     */
    control  = WRAPTOONE_21002;
#ifdef GYRO_DOUBLE_RATE
    control |= FS_DOUBLE_21002;
#endif
    gyro_write_block(CTRL_REG3_21002, control);
    if (gyro_validate_register(CTRL_REG3_21002, control))
        return -1;

    /*
     * FIFO Settings:
     *
     * Put the FIFO into circular buffer mode and set the watermark that will
     * trigger an interrupt.
     */
    // clear
    control = F_MODE_DISABLED_21002;
    gyro_write_block(F_SETUP_21002, control);
    if (gyro_validate_register(F_SETUP_21002, control))
        return -1;
    // set
    control = F_MODE_CIRCULAR_21002 | GYRO_FIFO_WATERMARK;
    gyro_write_block(F_SETUP_21002, control);
    if (gyro_validate_register(F_SETUP_21002, control))
        return -1;

    /* set active to start sampling */
    gyro_write_block(CTRL_REG1_21002, ODR_800_HZ_21002 | MODE_ACTIVE_21002);
    gyro.state = GYRO_NOT_CALIB;

    return 0;
}

void dump_gyro(gyro_t *gyro)
{
    printf("watermark: %d, calib_samples: %d\r\n", GYRO_FIFO_WATERMARK,
           GYRO_CALIB_SAMPLES);
    printf("calib_offset:\r\nx: %0.5f, y: %0.5f, z: %0.5f\r\n",
           gyro->calib_offset[0],
           gyro->calib_offset[1],
           gyro->calib_offset[2]);
    printf("latest status register: 0x%x\r\n", gyro->status);
    printf("samples: %u (%lu Hz), calib_samples: %u\r\n",
           gyro->samples,
           gyro->samples / ((ticks - gyro->samples_start_t) / 1000),
           gyro->calib_samples);
    printf("latest values:\r\nx: %0.2f, y: %0.2f, z: %0.2f\r\n",
           gyro->accum[0],
           gyro->accum[1],
           gyro->accum[2]);
}
