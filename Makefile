build: clean tetris.out
	# Build Complete ./tetris.out!

tetris.out:
	g++ -g $(wildcard src/*.cpp) -o tetris.out -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f tetris.out tetris.out
