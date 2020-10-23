1. Design decisions in the Brush class
  When computing masks, its side length is frequently referred, so I included m_maskSize in the Brush class.
Helper functions such as getIndex and and squaredDistance are also implemented.

2. Design decisions in the SmudgeBrush class
  I decided to use the quadratic distribution mask for SmudgeBrush, so I deleted makeMask() method in SmudgeBrush
and use inherited makeMask() from QuadraticBrush. Also, I added m_paint which is a vector indicating the paint
picked up from canvas. m_paint is initialized by initialzePaint().

3. Design decisions in the Canvas2D class
  I added member variables for Canvas2D, which are m_brushType, m_brushColor, m_currentRadius, and m_brush.
Last one is a smart pointer to the Brush object that is used currently. In the constructor method, I initialized
member values to null values using initializing list, and immediately updated using settingsChanged() in the
constructor body.w

4. Other decisions
  I decided to declare lerp as an unstatic function and included its function declaration in the Brush 
header file, so that SmudgeBrush can use it to paint when the mouse is dragged.

5. Bug
  When mouse is moved fast, smudgebrush leaves some discrete dots on the canvas. I suspect this occurs
because the computation of smudgebrush is not efficient enough.
