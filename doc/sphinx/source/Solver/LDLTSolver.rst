.. _sparse_ldlt_doc:
.. role:: important
.. role:: warning

<LDLTSolver />
==============

.. rst-class:: doxy-label
.. rubric:: Doxygen:
    :cpp:class:`SofaCaribou::solver::LDLTSolver`

Implementation of a sparse :math:`LDL^T` linear solver.


.. list-table::
    :widths: 1 1 1 100
    :header-rows: 1
    :stub-columns: 0

    * - Attribute
      - Format
      - Default
      - Description
    * - backend
      - option
      - Eigen
      - Solver backend to use.
            * **Eigen**
                | Eigen LDLT solver (SimplicialLDLT).
                | **[default]**

            * **Pardiso**
                Pardiso LLT solver.

Quick example
*************
.. content-tabs::

    .. tab-container:: tab1
        :title: XML

        .. code-block:: xml

            <Node>
                <StaticODESolver newton_iterations="10" correction_tolerance_threshold="1e-8" residual_tolerance_threshold="1e-8" printLog="1" />
                <LDLTSolver backend="Pardiso" />
            </Node>

    .. tab-container:: tab2
        :title: Python

        .. code-block:: python

            node.addObject('StaticODESolver', newton_iterations=10, correction_tolerance_threshold=1e-8, residual_tolerance_threshold=1e-8, printLog=True)
            node.addObject('LDLTSolver', backend="Pardiso")


Available python bindings
*************************

None at the moment.
