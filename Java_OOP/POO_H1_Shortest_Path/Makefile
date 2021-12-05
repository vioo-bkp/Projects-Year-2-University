JFLAGS = -g
JC = javac
.SUFFIXES: .java .class
build:
	$(JC) $(JFLAGS) cheapestPathInTheCity/*.java

run: build
	java cheapestPathInTheCity.Main
	
clean:
	rm cheapestPathInTheCity/*.class
