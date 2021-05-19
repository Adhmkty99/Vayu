#!/usr/bin/env python
from argparse import ArgumentParser
import concurrent.futures
from concurrent.futures import ProcessPoolExecutor
from dataclasses import dataclass
import logging
from pathlib import Path
import subprocess
import sys


@dataclass(frozen=True)
class TestResult:
    path: Path
    passed: bool
    output: str

    @property
    def failed(self) -> bool:
        return not self.passed

    def __str__(self) -> str:
        status = "PASS" if self.passed else "FAIL"
        output = f"\n{self.output}" if self.output else ""
        return f"{status}: {self.path}{output}"


def run_bundle(path: Path) -> TestResult:
    result = subprocess.run(
        [str(path)], stdout=subprocess.PIPE, stderr=subprocess.STDOUT
    )
    try:
        output = result.stdout.decode("utf-8")
    except UnicodeDecodeError:
        output = repr(result.stdout)
    return TestResult(path, result.returncode == 0, output)


class BundleRunner:
    def __init__(self, test_dir: Path) -> None:
        self.test_dir = test_dir

    def run_all(self) -> bool:
        failed = False
        with ProcessPoolExecutor() as pool:
            futures = []
            for bundle in self.test_dir.glob("**/*.bundle"):
                logging.debug("Submitting work for %s", bundle)
                futures.append(pool.submit(run_bundle, bundle))
            for future in concurrent.futures.as_completed(futures):
                result = future.result()
                if result.passed:
                    logging.info(result)
                else:
                    failed = True
                    logging.error(result)
        return failed


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
    if BundleRunner(args.test_dir).run_all():
        sys.exit("Tests failed")


if __name__ == "__main__":
    main()
