from setuptools import setup, Extension
from Cython.Build import cythonize
import os
import sys
import argparse
import PyInstaller.__main__

def build_cython_lib():
    # Define the extension module
    extensions = [
        Extension(
            name="_core", 
            sources=["lib/globalpycpp.pyx", "lib/core.c"],
            include_dirs=[os.path.dirname(os.path.abspath(__file__))],
        )
    ]

    # Setup configuration for library
    setup(
        name="libglobpycpp",
        version="0.1.0", 
        description="A small globalist library that works both in Python and C",
        author="Your Name",
        author_email="your.email@example.com",
        ext_modules=cythonize(extensions),
        packages=["libglobpycpp"],
        package_dir={"libglobpycpp": "./lib"},
    )

def build_executable(entry_point):
    PyInstaller.__main__.run([
        '--onefile',
        '--name=gblob',
        entry_point
    ])

def main():
    parser = argparse.ArgumentParser(description='Build tools for the globalist library')
    parser.add_argument('--lib', action='store_true', help='Build Cython library')
    parser.add_argument('--exe', help='Build executable from entry point')
    parser.add_argument('--clean', action='store_true', help='Clean build artifacts')
    
    args = parser.parse_args()

    if args.clean:
        # Clean build artifacts
        import shutil
        dirs_to_clean = ['build', 'dist', '__pycache__', '*.egg-info']
        for d in dirs_to_clean:
            try:
                shutil.rmtree(d)
            except:
                pass

    if args.lib:
        build_cython_lib()
        
    if args.exe:
        build_executable(args.exe)

    if not any(vars(args).values()):
        parser.print_help()

if __name__ == '__main__':
    main()
