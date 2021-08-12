#!/usr/bin/env python
from __future__ import annotations

from argparse import ArgumentParser
from collections.abc import Iterator
from contextlib import contextmanager
from datetime import timedelta
import logging
from pathlib import Path
import pickle
import subprocess
import sys
import timeit

from lxml.etree import Element, tostring
from lxml.builder import ElementMaker

from tradefed import TestDef


@contextmanager
def timed(event: str) -> Iterator[None]:
    start = timeit.default_timer()
    yield
    end = timeit.default_timer()
    duration = timedelta(seconds=end - start)
    logging.debug(f"{event}: {duration}")


class TfTestCaseBuilder:
    def __init__(self, defs: list[TestDef]) -> None:
        self.defs = defs
        self.builder = ElementMaker()

    def create_test_case(self, output: Path) -> None:
        output.write_bytes(
            tostring(
                self.create_config(),
                pretty_print=True,
                xml_declaration=True,
                encoding="utf-8",
            )
        )

    def create_config(self) -> Element:
        children = []
        children.append(self.create_test())

        return self.builder.configuration(*children, description="libc++ tests")

    def create_test(self) -> Element:
        return self.builder.test(
            *self.create_each_test(),
            **{"class": "com.android.tradefed.testtype.binary.ExecutableHostTest"},
        )

    def create_each_test(self) -> Iterator[Element]:
        for test_def in self.defs:
            yield self.builder.option(
                name="test-command-line",
                key=test_def.name,
                value=test_def.command,
            )

    @classmethod
    def from_directory(cls, path: Path) -> TfTestCaseBuilder:
        test_defs = []
        for def_path in path.glob("**/*.test.p"):
            with def_path.open("rb") as def_file:
                test_defs.append(pickle.load(def_file))
        return TfTestCaseBuilder(test_defs)


class TfRunner:
    def __init__(self, test_path: Path) -> None:
        self.test_path = test_path

    def run(self) -> bool:
        return (
            subprocess.run(
                [
                    "tradefed.sh",
                    "run",
                    "commandAndExit",
                    str(self.test_path),
                    "--null-device",
                ]
            ).returncode
            != 0
        )


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
    test_case = args.test_dir / "testcase.xml"
    with timed("Test XML generation"):
        TfTestCaseBuilder.from_directory(args.test_dir).create_test_case(test_case)
    with timed("Test execution"):
        failed = TfRunner(test_case).run()
    if failed:
        sys.exit("Tests failed")


if __name__ == "__main__":
    main()
