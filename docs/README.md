# Contributing to Project Documentation

This project leverages [Read the Docs](https://readthedocs.org/)'s
online hosting of [Sphinx](http://www.sphinx-doc.org/en/stable/)-generated
HTML assets from [reStructuredText](http://docutils.sourceforge.net/rst.html)
sources.

## Local Development Dependencies

* `python3` (Version 3.5.x+)
* `virtualenv` (Version 15.x.x+)
* `make` (Version 4.x)

A browser is also required for viewing the generated documentation.

### Windows-Specific

Instructions for getting the required software installed on Windows:

1. Download and install [Git Bash](https://git-scm.com/downloads). This
is where commands will be executed from.

2. Download and install [python3](https://www.python.org/downloads/)
(latest version is fine). Customize the installation options to
`Install for all users` and `Add Python to environment variables`.

3. Install `virtualenv`. From **Windows Command Prompt (as administrator)**
execute `python -m pip install --upgrade pip` then `pip install virtualenv`.

4. Set `PYTHON_BIN` environment variable. From **Git Bash** execute
`echo "export PYTHON_BIN='python'" >> ~/.bashrc && source ~/.bashrc`.

5. Download and install
[GNU Make](https://sourceforge.net/projects/gnuwin32/files/make/3.81/make-3.81.exe/download?use_mirror=ayera&download=). Follow the installation prompt and install to the default
location.

6. Alias `make` to its fully-qualified path. From **Git Bash** execute
`echo "alias make='/c/Program\ Files\ \(x86\)/GnuWin32/bin/make'" >> ~/.bashrc && source ~/.bashrc`.

7. From this directory with **Git Bash** execute `make venv` and ensure it executes
to completion to verify the installation. If this succeeds, execute `make html` to
build the documentation.

8. Set `BROWSER` so `make view` works correctly. From **Git Bash** execute
`echo "export BROWSER=/c/Program\ Files/Mozilla\ Firefox/firefox" >> ~/.bashrc && source ~/.bashrc`.
Note: replace `/c/Program\ Files/Mozilla\ Firefox/firefox` with a different path to
a browser of your choosing if desired.

9. Open a new instance of **Git Bash**, navigate to this directory and execute
`make serve`. To start the web server that hosts the documentation. Then execute
`make view` to have it opened in your browser.

Now, the local development workflow below should be functional.

## Local Development Workflow

From this directory, variations of `make <target>` invoked from
the command prompt can be used to perform documentation tasks.
Examples:

* `make help` - View `sphinx-build` targets
* `make serve` - Start a Python webserver on port 8000 to host the output
* `make view` - Open a `firefox` tab with the index page of the output
* `make html` - Generate the HTML output from the source files
