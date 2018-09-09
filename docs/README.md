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

TODO

## Local Development Workflow

From this directory, variations of `make <target>` invoked from
the command prompt can be used to perform documentation tasks.
Examples:

* `make help` - View `sphinx-build` targets
* `make serve` - Start a Python webserver on port 8000 to host the output
* `make view` - Open a `firefox` tab with the index page of the output
* `make html` - Generate the HTML output from the source files
