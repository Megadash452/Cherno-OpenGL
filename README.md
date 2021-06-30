## OpenGL

# Pull Instructions
After you pulling the repository, open the project (.sln) with Visual Studio. In the Solution Explorer, right click on the project and select **Properties**. Under the following categories of **Configuration Properties**, paste the follwing information:

### *C/C++ > General* > **Additional Include Directories**
> $(SolutionDir)dep;$(SolutionDir)dep\GLFW\include;$(SolutionDir)dep\GLEW\include;$(SolutionDir)src\HPP

### *C/C++ > Preprocessor* > **Preprocessor Definitions**
> *Click the* **Configuration** *dropper and select **DEBUG***
> - WIN32;\_CONSOLE;GLEW\_STATIC;%(PreprocessorDefinitions);\_DEBUG <br>
> *Click the* **Configuration** *dropper and select **RELEASE***
> - WIN32;\_CONSOLE;GLEW\_STATIC;%(PreprocessorDefinitions);NDEBUG

### *Linker > General* > **Additional Library Directories**
> $(SolutionDir)dep\GLFW\lib-vc2019;dep\GLEW\lib\Release\Win32
