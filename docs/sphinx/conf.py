# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

import os
import sys
import subprocess
sys.path.insert(0, os.path.abspath('../src'))

def run_doxygen():
    doxygen_path = os.path.abspath("../DoxyFile")  # Adjust based on the location
    if os.path.exists(doxygen_path):
        print("Running Doxygen...")
        subprocess.run(["doxygen", doxygen_path], check=True)
    else:
        print("DoxyFile not found. Skipping Doxygen step.")

run_doxygen()

project = 'HIT 2022 Analysis'
copyright = '2025, Lorenzo Pierfederici'
author = 'Lorenzo Pierfederici'
release = '0.0.1'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = ["breathe"]
breathe_projects = {
    "HIT 2022 Analysis": "../xml"
}
breathe_default_project = "HIT 2022 Analysis"

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

#extensions.append("sphinx_wagtail_theme")
#html_theme = 'sphinx_wagtail_theme'
html_theme ='alabaster'
html_static_path = ['_static']
