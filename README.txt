Hi, I am Andrew Ding.

Sources:

https://kartikkukreja.wordpress.com/2013/03/30/heuristic-function-for-reversiothello (Note: I did not look 
at the actual source code, only the mathematical formulas. It was especially useful because it provided a way to 
normalize the different measurements.)

http://www.cs.cornell.edu/~yuli/othello/othello.html (Description of a simple heuristic function)

Here are a list of heuristics I tried. They are all linear combinations of the "basic" heuristics (like stone 
difference, positional score, mobility score, etc.). I tested them all as black, supposedly the disadvantaged side, 
vs. the bots.

TIME-DEPENDENT NAIVE MIXTURE:
IF more than 35 empty squares
	1 positional score
ELSE IF more than 20 empty squares
	1 positional score
	40 stability score
	5 mobility score
ELSE
	1 stone parity
	
Results (fixed depth 4): win vs. BetterPlayer
Results (fixed depth 6): ~80% vs. ConstantTimePlayer, win vs. BetterPlayer
Results (fixed depth 8): win vs. BetterPlayer


NAIVE MIXTURE
1 adjusted stone parity
1 adjusted corner score
1 adjusted stability score

Results (fixed depth 6): ~80% vs. ConstantTimePlayer, loss vs. BetterPlayer
This isn't a bad result for a completely unscaled 1:1:1 mixture. It's notable, however, that calculating stablity
is extremely expensive.


CORNELL STUDENTS HEURISTIC
IF more than 14 empty squares
	1 stone parity
	100 mobility score
	1000 corner score
ELSE
	1 stone parity

Results (fixed depth 6): ~95% (19/20) vs SimplePlayer, ~50% vs. ConstantTimePlayer, win vs. BetterPlayer
Results (fixed depth 8): loss vs. BetterPlayer
It is highly probable that the 6-depth version's win vs. BetterPlayer was merely luck. BetterPlayer seems to play 
without randomness, so there's no way to test more for that version. However, it is notable that the 8-depth version
loss vs. BetterPlayer. That indicates that the heuristic is poor.
