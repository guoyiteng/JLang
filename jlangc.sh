./bin/jlangc -cp "$JDK"/out/classes ./tests/isolated/ConcurrentReflection.java -D playground/src \
&& ./bin/compile_ll.sh playground/src/ConcurrentReflection.ll \
&& ./playground/src/ConcurrentReflection.o $@
