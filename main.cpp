#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <cmath>
#include <sstream>

using namespace std;

class Fraction{
    private:
        int numerator, denominator;
        
    public:
        int getNumerator(){
            return numerator;
        }
        int getDenominator(){
            return denominator;
        }
        
        double decimal(){
            return (double)numerator / denominator;
        }
        
        Fraction reciprocal(){
            return Fraction(denominator, numerator);
        }
        void simplify(){
            if(numerator == 0){
                //0 / anything == 0
                if(!denominator == 0) denominator = 1;
            }
            else{
                int gcf = std::gcd(numerator, denominator);
                
                numerator /= gcf;
                denominator /= gcf;
                
                //Keep denominator positive
                if(denominator < 0){
                    numerator *= -1;
                    denominator *= -1;
                }
            }
        }
        
        //Support fraction addition, subtraction, *, /
        Fraction operator+(const Fraction& other){
            if(other.denominator == denominator) return Fraction(numerator + other.numerator, denominator);
            else{
                numerator = numerator * other.denominator + other.numerator * denominator;
                denominator =  denominator * other.denominator;
            }
            
            //Simplify :)
            simplify();
            
            return *this;
        }
        Fraction operator-(const Fraction& other){
            if(other.denominator == denominator) return Fraction(numerator + other.numerator, denominator);
            else{
                numerator = numerator * other.denominator - other.numerator * denominator;
                denominator =  denominator * other.denominator;
                
                
            }
            
            //Simplify :)
            simplify();
            
            return *this;
        }
        Fraction operator*(const Fraction& other){ 
            Fraction f = Fraction(numerator * other.numerator, denominator * other.denominator);;
            f.simplify();
            
            return f; 
        }
        Fraction operator/(const Fraction& other){
            Fraction f =  Fraction(numerator * other.denominator, denominator * other.numerator);
            f.simplify();
            
            return f; 
        }
        
        Fraction& operator+=(const Fraction& other){
            if(other.denominator == denominator){
                numerator += other.numerator;
            }
            else{
                numerator = numerator * other.denominator + other.numerator * denominator;
                denominator =  denominator * other.denominator;
            }
            
            simplify();
            
            return *this;
        }
        Fraction& operator*=(const Fraction& other){
            numerator *= other.numerator;
            denominator *= other.denominator;
            
            simplify();
            
            return *this;
        }
        Fraction operator-(){
            return Fraction(-numerator, denominator);
        }
        
        bool operator==(const Fraction& other){
            return numerator == other.numerator && denominator == other.denominator;
        }
        bool operator==(const int other){
            //cout << decimal() << " == " << other << " " << (other == decimal()) << endl;
            return other == decimal();
        }
        bool operator==(const double other){
            return other == decimal();
        }
        bool operator==(const float other){
            return other == decimal();
        }
        
        
        
        void print(){
            if(denominator == 1) cout << numerator;
            else cout << numerator << "/" << denominator;
        }
        
        
        Fraction(int numerator, int denominator){
            this->numerator = numerator;
            this->denominator = denominator;
        }
        Fraction(int val){
            numerator = val;
            denominator = 1;
        }
};

class Matrix{
    protected:
        int numRows, numCols;
        
        vector<vector<Fraction>> matrix;
        
        //Assign a non-empty matrix
        void assignArr(vector<vector<Fraction>> matrix){
            this->matrix = matrix;
            
            
            numRows = matrix.size();
            numCols = matrix.at(0).size();
        }
        void addRow(vector<Fraction> row){
            matrix.push_back(row);
        }
        
    public:
        vector<vector<Fraction>> getArr(){ return matrix; }
    
        int getNumRows(){
            return numRows;
        }
        int getNumCols(){
            return numCols;
        }
        
        Fraction getEntry(int r, int c){
            //cout << "Fetch "<<r<<","<<c<<" size "<<matrix.size()<<","<<matrix.at(0).size()<<endl;
            return matrix.at(r).at(c);
        }
        void setEntry(int r, int c, Fraction entry){
            matrix.at(r).at(c) = entry;
        }
        
        
        void print(){
            cout << "[ ";
            
            for(int r = 0; r < numRows; r++){
                for(int c = 0; c < numCols; c++){
                    getEntry(r, c).print();
                    cout << ", ";
                }
                //Newline if not last row
                if(r < numRows - 1) cout << "\n  ";
            }
            
            cout << " ]" << endl;
        }
        
        
        Matrix(int numRows, int numCols){
            this->numRows = numRows;
            this->numCols = numCols;
            
            //Fill matrix with zeros so undefined
            //0/0 will be undefined
            for(int r = 0; r < numRows; r++){
                vector<Fraction> tmp;
                for(int c = 0; c < numCols; c++){
                    tmp.push_back(Fraction(0, 0));
                }
                matrix.push_back(tmp);
            }
        }
        Matrix(Matrix& m){
            assignArr(m.getArr());
        }
        Matrix(){}; //Default constructor
};

class RowEquMatrix : public Matrix{
    public:
        int numSwaps = 0;
        //Keep track of which rows have been reduced
        bool reducedRows[];
    
        RowEquMatrix(int numRows, int numCols) : Matrix(numRows, numCols){}
        /*RowEquMatrix(string s){
            //Split along space

            stringstream ss(s);
            string token;
            int intsFound = 0;
            int n;
            bool tokenHasInt;
            while (getline(ss, token, ' ')) {
                tokenHasInt = true;
                
                cout<<token<<endl;
                
                try{
                    n = stoi(token);
                }
                catch(const std::invalid_argument& e){tokenHasInt = false;}
                
                if(tokenHasInt && to_string(tokenHasInt).size() == token.size()){
                    if(intsFound == 0) numRows = n;
                    else if (intsFound == 1){
                        numCols = n;
                        
                        //Fill vectors
                        //Fill matrix with zeros so undefined
                        //0/0 will be undefined
                        for(int r = 0; r < numRows; r++){
                            vector<Fraction> tmp;
                            for(int c = 0; c < numCols; c++){
                                tmp.push_back(Fraction(0, 0));
                            }
                            matrix.push_back(tmp);
                        }
                    }
                    else{
                        int ePos = intsFound - 2;
                        cout<<"Index: "<<ePos<<": "<<n<<endl;
                        setEntry(ePos / numCols, ePos % numCols, Fraction(n));
                    }
                    intsFound++;
                }
                
            }
            
            
        }
        */
        void multAndAdd(int fromRow, double mult, int toRow){multAndAdd(fromRow, Fraction(mult), toRow);}
        void multAndAdd(int fromRow, Fraction mult, int toRow){
            //cout << "* row " << fromRow << " by " << mult.decimal() << " and add that to row " << toRow << endl;
            for(int i=0; i<numCols; i++){
                matrix.at(toRow).at(i) += matrix.at(fromRow).at(i) * mult;
            }
            
            reducedRows[toRow] = true;
        }
        void scaleRow(double scale, int row) {scaleRow(Fraction(scale), row);}
        void scaleRow(Fraction scale, int row){
            for(int i=0; i<numCols; i++){
                matrix.at(row).at(i) *= scale;
            }
            
            reducedRows[row] = true;
        }
        
        //Sorts rows to get closer to echelon form
        void sort(){
            //# of zeros before leading entry in each row
            int priority[numRows];
            
            for(int r=0; r<numRows; r++){
                int c=0;
                while(getEntry(r,c) == 0){
                    c++;
                    if(c == numCols) break;
                }
                
                priority[r] = c;
                //cout << "Row " << r << ": " << c << endl;
            }
            
            //Insertion sort
            Matrix copy = Matrix(*this);
            matrix.clear();
            
            //Add a row to empty matrix
            addRow(copy.getArr().at(0));
            vector<int> priorities = {priority[0]};
            
            for(int r=1; r<copy.getNumRows(); r++){
                //Figure out where to insert
                int i=0;
                while(i<priorities.size()){
                    if(priorities.at(i) > priority[r]) break;
                    i++;
                }
                //Insert
                matrix.insert(matrix.begin()+i, copy.getArr().at(r));
                priorities.insert(priorities.begin()+i, priority[r]);
                
                swap(i, r);
            }
            
            //cout <<"Sorted: "<<endl;
            //print();
            //cout<<endl;
        }
    protected:
        //Helper for matrix solver
        void swap(int a, int b){
            bool tmp = reducedRows[a];
            reducedRows[a] = reducedRows[b];
            reducedRows[b] = tmp;
            
            numSwaps++;
        }
};

class MatrixSolver{
    public:
        //Precondition: leading entry of each row must be in same or left col of LE of row below
        static void echelonForm(RowEquMatrix& matr, bool showSteps = true, bool scaleRows = true){
            //echelon form w/o scaling: don't do anything to 1st row
            if(!scaleRows) matr.reducedRows[0] = true;
            
            
            
            //Keep rows close to echelon form to prevent bugs
            matr.sort();
            if(showSteps) {cout << "\nSorted:"<<endl;
            matr.print();}
            
            int r = 0;
            while(r < matr.getNumRows()){
                
                for(int c=0; c<matr.getNumCols(); c++){
                    if(showSteps) cout << "New col: "<<c <<"\n\n";
                    
                    //cout << "Get entry at "<<r<<","<<c<<endl;
                    Fraction thisEntry = matr.getEntry(r,c);

                    //If next entry is 0 go over a column
                    if(thisEntry == 0){
                        continue;
                    }
                    
                    //scale leadng entry so 1
                    if(scaleRows){
                        matr.scaleRow(thisEntry.reciprocal(), r);
                        
                        if(showSteps) {cout << "Scale row " << r << " to 1:" << endl;
                        matr.print();}
                    }
                    
                    //All done!
                    if(r == matr.getNumRows() - 1) break;
                    
                    const int myRow = r;
                    
                    r++;
                    //Turn entries below LE to 0s
                    while(r < matr.getNumRows()){
                        thisEntry = matr.getEntry(r,c);
                        if(showSteps){cout << "Multiply row " << (myRow+1) << " by ";
                        (-thisEntry / matr.getEntry(myRow,c)).print();
                        cout << " and add to row " << (r+1) << " to make col " << (c+1) << " a 0:" << endl;}
                        
                        matr.multAndAdd(myRow, -thisEntry / matr.getEntry(myRow,c), r);
                        if(showSteps) matr.print();
                        
                        r++;
                    }
                    r = myRow+1;
                }
                
                //Sort matrix to push cancelled rows to the bottom
                matr.sort();
                if(showSteps){cout << "\nSorted:"<<endl;
                matr.print();}
                
                //Go back to highest un-reduced row
                for(int i=0; i<matr.getNumRows(); i++){
                    r = i;
                    if(!matr.reducedRows[r]) break;
                    if(i == matr.getNumRows() - 1) r = matr.getNumRows(); //Exit
                }
                if(showSteps) cout << "Next row: "<<r<<endl;
            }
        }
        
        //Precondition: matrix is in echelon form and all leading entries = 1
        static void reducedEchelonForm(RowEquMatrix& matr, bool showSteps = true){
            for(int r=matr.getNumRows()-1; r>=0; r--){
                //Find leading entry
                int c=0;
                Fraction thisEntry = Fraction(0,1);
                while(thisEntry == 0){
                    c++;
                    if(c == matr.getNumCols()) break;
                    
                    thisEntry = matr.getEntry(r,c);
                    /*cout << "At " << r << "," << c << ":";
                    thisEntry.print();
                    cout << endl;*/
                }
                
                if(thisEntry==0) continue; //This row is all 0s
                
                //Reduce above rows
                for(int i=r-1; i>=0; i--){
                    if(showSteps) {cout << "Multiply row " << (r+1) << " by ";
                    (-matr.getEntry(i, c)).print();
                    cout << " and add to row " << (i+1) << " to make col " << (c+1) << " a 0:" << endl;}
                    
                    matr.multAndAdd(r, -matr.getEntry(i, c), i);
                    if(showSteps) matr.print();
                }
            }
        }
};

int main()
{
    cout << "Hello World!\n\n";
    
    
    //Create matrix: ask for size
    int r, c, entry;
    string thisEntry;
    
    cout << "Num rows? " <<endl;
    cin >> r;
    
    cout << "Num columns? "<<endl;
    cin >> c;
    
    RowEquMatrix matr = RowEquMatrix(r, c);
    RowEquMatrix matr2 = RowEquMatrix(r, c);
    
    cin.ignore();
    
    //Ask user to fill in matrix
    for(int i = 0; i<r; i++){
        for(int j = 0; j<c; j++){
            matr.print();
            cout << "What is row " << i+1 << " col " << j+1 << "?\n";
            
            getline(cin, thisEntry);
            int delim = thisEntry.find('/');
            if(delim != std::string::npos){
                //cout << "num: "<<thisEntry.substr(0, delim)<<endl;
                //cout << "denom: "<<thisEntry.substr(delim+1)<<endl;
                int numerator = std::stoi(thisEntry.substr(0, delim));
                int denominator = std::stoi(thisEntry.substr(delim+1));
                matr.setEntry(i, j, Fraction(numerator, denominator));
                matr2.setEntry(i, j, Fraction(numerator, denominator));
            } else {
                //Not fraction: Integer was inputted
                //cout << "Not fraction: "<<thisEntry<<endl;
                matr.setEntry(i, j, Fraction(std::stoi(thisEntry)));
                matr2.setEntry(i, j, Fraction(std::stoi(thisEntry)));
            }
        }
    }
    
    /*cout<<"Paste matrix: ";
    cin.ignore();
    string matrixStr = "Start 4 By 5 Table 1st Row 1st Column 1 2nd Column 1 3rd Column 5 4st Column negative 12 5st Column 6 2nd Row 1st Column 1 2nd Column 1 3rd Column 0 4st Column 3 5st Column 6 3rd Row 1st Column 2 2nd Column 2 3rd Column negative 2 4st Column 12 5st Column 9 4st Row 1st Column 4 2nd Column 4 3rd Column 0 4st Column 12 5st Column 9 EndTable";
    //getline(cin, matrixStr);
    
    RowEquMatrix matr = RowEquMatrix(matrixStr);
    RowEquMatrix matr2 = RowEquMatrix(matrixStr);*/
    
    
    cout << "--------------------------------------------\n\nYour matrix:\n";
    
    matr.print();

    MatrixSolver::echelonForm(matr);
    
    cout << "\n-----------Echelon Form:\n";
    matr.print();
    
    MatrixSolver::reducedEchelonForm(matr);
    
    cout << "\n-----------Reduced Echelon Form:\n";
    matr.print();
    
    //Find determinant through row ops
    if(matr2.getNumRows() == matr2.getNumCols()){
        MatrixSolver::echelonForm(matr2, false, false);
        
        Fraction det = Fraction(1);
        
        //Multiply down main diagonal
        for(int i = 0; i<matr2.getNumRows(); i++){
            det *= matr2.getEntry(i,i);
        }
        
        //Negate row interchanges
        det *= pow(-1, matr2.numSwaps);
        
        cout << "\n----\nDeterminant: ";
        det.print();
    }
    
    
    return 0;
}