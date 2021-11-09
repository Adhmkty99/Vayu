#include "llvm/Passes/PassPlugin.h"
#define HANDLE_EXTENSION(Ext)                                                  \
		llvm::PassPluginLibraryInfo get##Ext##PluginInfo();
#include "llvm/Support/Extension.def"


namespace llvm {
	namespace details {
		void extensions_anchor() {
#define HANDLE_EXTENSION(Ext)                                                  \
<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
			static auto Ext = get##Ext##PluginInfo();
=======
			get##Ext##PluginInfo();
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)
#include "llvm/Support/Extension.def"
		}
	}
}
