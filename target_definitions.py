from pathlib import Path
from typing import List

from cmake_generator import NewTarget, Target, TargetType

# ----------------------------------------------------------------
def get_target_definitions() -> List[ Target ] :

    """
    We define almost exactly the same target twice.
    Once as a shared library, which can be used to create Python bindings,
    and once as an executable, which can be used to run the engine as a regular c++ executable.
    """

    src_dir_path = (Path( __file__ ).resolve().parent / 'src/').as_posix()

    dependencies = [
        'glm',
        'json11',
        'shake_content',
        'shake_core',
        'shake_ecs',
        'shake_graphics',
        'shake_hid',
        'shake_io'
    ]

    # Shared library, which can be used to create Python bindings.
    shake_main = NewTarget(
        name = 'shake_main',
        target_type = TargetType.SharedLibrary,
        src_dir_path = src_dir_path,
        dependencies = dependencies
    )

    # Executable, which can be used to run the engine as a regular c++ executable.
    shake_exe = NewTarget(
        name = '_shake3',
        target_type = TargetType.Executable,
        src_dir_path = src_dir_path,
        dependencies = dependencies
    )

    return [ shake_main, shake_exe ]
