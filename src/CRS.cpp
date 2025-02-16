using namespace std;
class CRM{
	private:
		vector<vector<float>> originalMatrix;
		vector<int> cols, row_ptrs; 
		vector<float> vals; // non zero values
	public:
	
		CRM(vector<vector<float> matrix){
		 this.originalMatrix = matrix;
		}

		vector<vector<float>> getOriginalMatrix(){
			return this.originalMatrix;
		}
		
		// :TODO
		// convert sparce matrix to CRS format
		// print origianl matrix
		// multiplyMatrices
		//
		void convert(){}
		void printOriginal(){}
		CRM operator*(CRM const& m){}
		void uvalue(int row, int col, int new_val){}//update value
		
}
