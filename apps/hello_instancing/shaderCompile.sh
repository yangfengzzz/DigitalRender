../../../bgfx/.build/osx64_clang/bin/shadercRelease -f vs_instancing.sc -o vs_instancing.bin -i ../../src.bgfx/bgfx --type v --platform osx -p metal
../../../bgfx/.build/osx64_clang/bin/shadercRelease -f vs_skybox.sc -o vs_skybox.bin -i ../../src.bgfx/bgfx --type v --platform osx -p metal
../../../bgfx/.build/osx64_clang/bin/shadercRelease -f fs_skybox.sc -o fs_skybox.bin -i ../../src.bgfx/bgfx --type f --platform osx -p metal

../../../bgfx/.build/osx64_clang/bin/shadercRelease -f fs_blinn.sc -o fs_blinn.bin -i ../../src.bgfx/bgfx --type f --platform osx -p metal
../../../bgfx/.build/osx64_clang/bin/shadercRelease -f fs_two.sc -o fs_two.bin -i ../../src.bgfx/bgfx --type f --platform osx -p metal
../../../bgfx/.build/osx64_clang/bin/shadercRelease -f fs_one.sc -o fs_one.bin -i ../../src.bgfx/bgfx --type f --platform osx -p metal
