#!/usr/bin/env python
from argparse import ArgumentParser
import logging
from pathlib import Path
import subprocess
import sys


class TfRunner:
    def __init__(self, test_dir: Path) -> None:
        self.test_dir = test_dir

    def run_all(self) -> bool:
        cmd_file = self.create_command_file()
        return (
            subprocess.run(
                ["tradefed.sh", "run", "cmdfileAndExit", str(cmd_file)]
            ).returncode
            != 0
        )

    def create_command_file(self) -> Path:
        command_file = self.test_dir / "cmdfile.txt"
        command_file.write_text(
            "\n".join(str(p) for p in self.test_dir.glob("**/*.xml"))
        )
        return command_file


def main() -> None:
    parser = ArgumentParser()
    parser.add_argument("-v", "--verbose", action="count", default=0)
    parser.add_argument("test_dir", type=Path)
    args = parser.parse_args()

    if args.verbose >= 2:
        level = logging.DEBUG
    elif args.verbose == 1:
        level = logging.INFO
    else:
        level = logging.WARNING
    logging.basicConfig(level=level)
    if TfRunner(args.test_dir).run_all():
        sys.exit("Tests failed")


if __name__ == "__main__":
    main()
