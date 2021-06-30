## OpenGL

# Pull Instructions
After you pulling the repository, open the project (.sln) with Visual Studio. In the Solution Explorer, right click on the project and select **Properties**. Under the following categories of **Configuration Properties**, paste the follwing information (make sure you are editing *ALL CONFIGURATIONS*:

#### *C/C++ > General* > **Additional Include Directories**
> $(SolutionDir)dep;$(SolutionDir)dep\GLFW\include;$(SolutionDir)dep\GLEW\include;$(SolutionDir)src\HPP

#### *C/C++ > Preprocessor* > **Preprocessor Definitions**
> *Click the* **Configuration** *dropper and select **DEBUG***
>> WIN32;\_CONSOLE;GLEW\_STATIC;%(PreprocessorDefinitions);\_DEBUG <br>

> *Click the* **Configuration** *dropper and select **RELEASE***
>> WIN32;\_CONSOLE;GLEW\_STATIC;%(PreprocessorDefinitions);NDEBUG <br>
- *Click the* **Configuration** *dropper and select **ALL CONFIGURATIONS***

#### *Linker > General* > **Additional Library Directories**
> $(SolutionDir)dep\GLFW\lib-vc2019;dep\GLEW\lib\Release\Win32

#### *Linker > Input* > **Additional Dependencies**
> glfw3.lib;glew32s.lib;opengl32.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)
