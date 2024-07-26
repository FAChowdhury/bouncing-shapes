<h1>Bouncing Shapes</h1>
<img src="./demo/2024-07-26 17-29-06 - Trim.gif">
<h2>Requirements</h2>
<ul>
  <li>clang++</li>
  <li>clang-format</li>
  <li>cmake</li>
  <li>SFML</li>
</ul>
<h2>Goal</h2>
<p>
  Create a program in C++ that animates shapes bouncing around a window using SFML. The shapes are configured from <code>./src/config.txt</code>.
  Users are able to use the ImGui interface to change the properties of shapes dynamically while the programming is running.

  **THIS IS A WIP.**
</p>
<h2>Linting using clang-format</h2>
<code>find . -iname "*.h" -o -iname "*.cpp" | xargs clang-format -i</code>
<h2>Building the project</h2>
<h3>To build the game, complete the following steps:</h3>
<ol>
    <li>
      Run <code>cmake -B build</code> on the root directory.
    </li>
    <li>
      Run <code>cd build</code>
    </li>
    <li>
      Run <code>make</code> to build the project
    </li>
    <li>
      Run <code>./GAME</code>
    </li>
</ol>
<h2>Interpreting src/config.txt</h2>
<h3>src/config.txt will take the following format:</h3>
<code>Window Width(int) Height(int)
Shape(string) Name(string) X_Position(float) Y_Position(float) X_Velocity(float) Y_Velocity(float) R(int) G(int) B(int) Properties(float)
</code>
<p>Shape can either be a circle or a rectangle.</p>
<p>The properties will be the following.</p>
<h3>For a circle:</h3>
<code>Radius(float)</code>
<h3>For a rectangle:</h3>
<code>Width(float) Height(float)</code>

