# iRoot
## An easy-to-use front-end for analysing ROOT TTree structures
 
iRoot was developed during the R&D phase of the INO-ICAL prototype stack at Tata Institute of Fundamental Research. Most data formats were in binary at that time and we decided to shift to TTree format owing to the easy plotting and analysis features of ROOT that came along with it.
In due course, we found that is most analysis, plots have to be made from mulitple files, looped over some array index. Also, a visual assessment of the data values was important at that time. 
To address the requirements of multiple groups, I designed a front-end for ROOT TTree structures which can be used just like MS-Excel. 
The tool can be used to:
a) Open multiple TFiles with TTrees inside and view them in a Table (see screenshot2.png)
b) Loop over array like structures and plot them in a separate canvases (or superimpose)
c) Fit data to simple functions 
d) Edit the plot style like color / fonts / scales etc.

and more importantly, all in a single GUI.

Your feedback is important. Please write to me at deepaksamuel@gmail.com
