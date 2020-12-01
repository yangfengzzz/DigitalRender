../../../bgfx/.build/osx64_clang/bin/shadercRelease -f vs_model.sc -o vs_model.bin -i ../../src.bgfx/bgfx --type v --platform osx -p metal

../../../bgfx/.build/osx64_clang/bin/shadercRelease -f fs_two.sc -o fs_two.bin -i ../../src.bgfx/bgfx --type f --platform osx -p metal
../../../bgfx/.build/osx64_clang/bin/shadercRelease -f fs_one.sc -o fs_one.bin -i ../../src.bgfx/bgfx --type f --platform osx -p metal
../../../bgfx/.build/osx64_clang/bin/shadercRelease -f fs_basic.sc -o fs_basic.bin -i ../../src.bgfx/bgfx --type f --platform osx -p metal
