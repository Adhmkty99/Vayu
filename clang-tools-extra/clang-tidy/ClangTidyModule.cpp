//===--- tools/extra/clang-tidy/ClangTidyModule.cpp - Clang tidy tool -----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
///  \file Implements classes required to build clang-tidy modules.
///
//===----------------------------------------------------------------------===//

#include "ClangTidyModule.h"
#include "ClangTidyCheck.h"

namespace clang {
namespace tidy {

void ClangTidyCheckFactories::registerCheckFactory(StringRef Name,
                                                   CheckFactory Factory,
                                                   bool IsAllFileCheck) {
  Factories.insert_or_assign(Name, std::move(Factory));
  IsAllFileChecks.insert_or_assign(Name, IsAllFileCheck);
}

std::vector<std::unique_ptr<ClangTidyCheck>>
ClangTidyCheckFactories::createChecks(ClangTidyContext *Context) {
  std::vector<std::unique_ptr<ClangTidyCheck>> Checks;
  for (const auto &Factory : Factories) {
    if (Context->isCheckEnabled(Factory.getKey()) &&
        !IsAllFileChecks[Factory.getKey()])
      Checks.emplace_back(Factory.getValue()(Factory.getKey(), Context));
  }
  return Checks;
}

std::vector<std::unique_ptr<ClangTidyCheck>>
ClangTidyCheckFactories::createAllFileChecks(ClangTidyContext *Context) {
  std::vector<std::unique_ptr<ClangTidyCheck>> AllFileChecks;
  for (const auto &Factory : Factories) {
    if (Context->isCheckEnabled(Factory.getKey()) &&
        IsAllFileChecks[Factory.getKey()])
      AllFileChecks.emplace_back(Factory.getValue()(Factory.getKey(), Context));
  }
  return AllFileChecks;
}

ClangTidyOptions ClangTidyModule::getModuleOptions() {
  return ClangTidyOptions();
}

} // namespace tidy
} // namespace clang
