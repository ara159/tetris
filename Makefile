build: clean app.out
	echo ok

app.out:
	g++ $(wildcard src/*.cpp) -o app.out -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f app.out