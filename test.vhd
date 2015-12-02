"100"

LIBRARY ieee;  -- this is comment

USE ieee.std_logic_1164.all;

ENTITY OddThing IS

 PORT (x1, x2, x3, x4: IN STD_LOGIC;

      f : OUT STD_LOGIC); --comment2

END OddThing;

 

ARCHITECTURE LogicFunction OF OddThing IS --comment here!

 

SIGNAL  a : STD_LOGIC;

SIGNAL  b : STD_LOGIC;

BEGIN

 Object1: entity work.light port map(x1=>x1,x2=>x2,f=>a);

 

 Object2: entity work.light port map(x1=>x3,x2=>x4,f=>b);

 Object3: entity work.light port map(x1=>'a',x2=>b,f=>f);

END LogicFunction;