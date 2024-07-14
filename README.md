<h1>Bouncing Shapes</h1>
<h2>Requirements</h2>
<ul>
  <li>clang++</li>
  <li>clang-format</li>
  <li>cmake</li>
  <li>SFML</li>
</ul>
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
<code>
  Window Width Height
  Shape Name X_Position Y_Position X_Velocity Y_Velocity R G B Properties
</code>
<p>Shape can either be a circle or a rectangle.</p>
<p>The properties will be the following.</p>
<p>For a circle:</p>
<code>Radius</code>
<p>For a rectangle:</p>
<code>Width Height</code>

