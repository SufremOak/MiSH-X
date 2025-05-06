import subprocess
import os
import sys
import argparse
import shutil

def MakeEnv(ext):
    # Create a virtual environment
    subprocess.check_call([sys.executable, "-m", "venv", "venv"])
    
    # Activate the virtual environment
    activate_script = os.path.join("venv", "Scripts", f"activate{ext}")
    if not os.path.exists(activate_script):
        raise FileNotFoundError(f"The activate script '{activate_script}' does not exist.")
    print(f"Activate script: {activate_script}")

    # Install required packages
    subprocess.check_call([sys.executable, "-m", "pip", "install", "-r", "requirements.txt"])


def CleanEnv():
    # Remove the virtual environment directory
    if os.path.exists("venv"):
        shutil.rmtree("venv")
        print("Virtual environment cleaned.")
    else:
        print("No virtual environment found to clean.")


def main():
    parser = argparse.ArgumentParser(description="Create or clean a virtual environment.")
    parser.add_argument("--create", action="store_true", help="Create a new virtual environment.")
    parser.add_argument("--clean", action="store_true", help="Clean the existing virtual environment.")
    parser.add_argument("--ext", type=str, choices=[".ps1", ".fish", ".csh", ".bat"], default=".bat",
                        help="The file extension of the activate script (default: .bat)")

    args = parser.parse_args()

    if args.create:
        MakeEnv(args.ext)
    elif args.clean:
        CleanEnv()
    else:
        parser.print_help()


if __name__ == "__main__":
    main()
