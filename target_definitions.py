from pathlib import Path
from typing import List

from cmake_generator import NewTarget, Target, TargetType

# ----------------------------------------------------------------
def get_target_definitions() -> List[ Target ] :
    shake_main = NewTarget(
        name = 'shake_main',
        target_type = TargetType.SharedLibrary,
        src_dir_path = ( Path( __file__ ).resolve().parent / 'src/' ).as_posix(),
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
    )
    return [ shake_main ]
