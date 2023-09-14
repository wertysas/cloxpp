import os.path
DIR_OF_THIS_SCRIPT = os.path.abspath( os.path.dirname( __file__ ) ) 

def Settings( **kwargs ):
    cmake_commands = os.path.join( DIR_OF_THIS_SCRIPT, 'cmake-build-release', 'compile_commands.json')
    if os.path.exists( cmake_commands ):
        return {
        'ls': {
            'compilationDatabasePath': os.path.dirname( cmake_commands )
        }
        }
    return None
