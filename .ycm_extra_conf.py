def FlagsForFile( filename, **kwargs ):
    return {
        'flags': [
            '-I',
            './inc',
            '-std=c99',
            '-Wall',
            '-Wextra',
            '-Werror',
        ],
    }
