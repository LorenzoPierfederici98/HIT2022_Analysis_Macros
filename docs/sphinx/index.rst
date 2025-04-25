.. HIT2022 Analysis documentation master file, created by
   sphinx-quickstart on Sun Jan 26 18:17:18 2025.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to HIT2022 Analysis's documentation!
============================================

Overview
--------
This project contains a set of macros used to analyze Calorimeter and TOF-wall data from the HIT2022 campaign, focus of my MS thesis work. These macros take as input the ROOT output files produced by the macros which analyze the runs decoded by the SHOE software, producing the histograms of interest. The ROOT output files of the runs characterized by the same beam energy value are merged into a single file, which is then used as input for these macros.

Structure
---------
- **Calorimeter Analysis** – Macros for the analysis of the energetic response of the crystals and resolution evaluation.
- **TOF-Wall Analysis** – Macros for the energy and Time-Of-Flight (TOF) calibrations, resolution evaluation.

Directory Structure
-------------------

Typical structure of the macros and input files used in the analysis:
.. code-block:: text

   src/                    # Path of the analysis macros source code
   ├── TW/cuts/            # TW uncalibrated files, with the selections to remove pedestal and pile-up
   ├── Calo/               # Calorimeter-related input files
   ├── Plots/              # Output plots and figures

Run
---

To run the macros, ROOT needs to be installed and set up on the system. The macros are written in C++ and can be executed within the ROOT environment.

.. code-block:: bash

   root -l -b 'AnalyzePeakCrystal.cc()'


.. toctree::
   :maxdepth: 2
   :caption: Contents:

   api


Documentation Links
-------------------

- :ref:`API Reference <api>`
- `GitHub Repository <https://github.com/LorenzoPierfederici98/HIT2022_Analysis_Macros>`_

Dependencies
------------

- `ROOT <https://root.cern>`_

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
