make -f $PARTICLE_MAKEFILE compile-user
timeout 3 particle call crazy_boomer enterDfuMode 1
make -f $PARTICLE_MAKEFILE flash-user
