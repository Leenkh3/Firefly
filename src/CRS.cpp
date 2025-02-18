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
		void convert(){
			row_ptrs.push_back(1); // TODO: know why it starts from 1 ??
			row_ptrs.clear(); cols.clear(); vals.clear();
			for(const auto& row :originalMatrix){
				for(int col_idx=0;col_idx<row.size();col_idx++){
					if(row[col_index]!=0.0){
						cols.push_back(col_idx+1);
						vals.push_back(row[col_idx]);
					}
					
				}
			row_ptrs.push_back(cols.size() + 1);
			
			}
		}
		void printOriginal(){}
		CRM operator*(CRM const& m){}
		void uvalue(int row, int col, int new_val){}//update value
		
}
