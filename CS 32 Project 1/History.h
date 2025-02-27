#ifndef History_h
#define History_h
#include "globals.h"
class History
    {
      public:
        History(int nRows, int nCols);
        bool record(int r, int c);
        void display() const;
        
      private:
        int m_rows;
        int m_cols;
        char m_history[MAXROWS][MAXCOLS];
    };
#endif /* History_hpp */
