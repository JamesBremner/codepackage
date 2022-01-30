
# compiler include search paths
INCS = -I./ \
	-I../../boost/boost1_72 \
	-I../../raven-set \
	-I../../windex/include

# folders

SDIR = ./src
ODIR = ./.vscode/obj

# symbols

OPT = -O2
CCflags = -std=c++17 $(OPT)
STATLibs = -static-libstdc++ -static-libgcc -lUrlmon -static
BOOSTLIB = -L../../boost/boost1_72/lib -lboost_program_options-mgw82-mt-x64-1_72
WEXLIBS = -lstdc++fs -lgdiplus -lgdi32 -lcomdlg32

### OBJECT FILES

OBJ = $(ODIR)/pack.o $(ODIR)/cPackage.o

### COMPILE

$(ODIR)/%.o : $(SDIR)/%.cpp
	g++ -o  $@ -c $< $(INCS) $(CCflags)

#### LINK

pack : $(OBJ)
	g++ -o ./bin/pack.exe \
		$(OBJ) $(WEXLIBS) $(STATLibs) $(OPT)

### CLEAN

.PHONY: clean
clean:
	del /q .\.vscode\obj\*.o