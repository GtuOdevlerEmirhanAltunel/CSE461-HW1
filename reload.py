import os
import subprocess
from time import sleep


def compile_and_run():
    # Change the directory to the location of the script
    script_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(script_dir + "/build")
    # Compile the script
    subprocess.run(["make"])
    # Run the compiled script
    subprocess.run(["./RayTracer"])


def main():
    last_build_time = 0

    while True:
        files = os.listdir(
            "/home/demir/Documents/Projects/CSE461-HW1/src"
        ) + os.listdir("/home/demir/Documents/Projects/CSE461-HW1/inc")

        # Check if any source files have been modified
        for file in files:
            if os.path.exists(
                "/home/demir/Documents/Projects/CSE461-HW1/src/" + file
            ):
                file_m = os.path.getmtime(
                    "/home/demir/Documents/Projects/CSE461-HW1/src/" + file
                )
            else:
                file_m = os.path.getmtime(
                    "/home/demir/Documents/Projects/CSE461-HW1/inc/" + file
                )
            if file_m > last_build_time:
                last_build_time = file_m
                print(f"Detected changes in {file}. Recompiling...")
                compile_and_run()
                break
        sleep(1)


if __name__ == "__main__":
    main()
