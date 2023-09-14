from distutils.core import setup, Extension
from os import environ

# Set g++ compiler
environ["CC"] = "g++"

def main():
    setup(name="Teracada",
          version="1.1.1",
          description="Teracada",
          author="Rishabh Soni",
          ext_modules=[Extension(
                        "Teracada",
                        [
                          "teracada_pyapi.c",
                          "pyapi_helper.c",
                          "pyapi_array.c",
                          "pyapi_regression.c"
                        ],
                        include_dirs=["../include", "../../logman"],
                        library_dirs = ["../artifacts"],
                        libraries = ["teracada64"],
                        extra_compile_args = ["-Wno-unused-label", "-Wno-write-strings"]
                        )])

if __name__ == "__main__":
    main()