#include <iostream> 
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <thread>
#include <future>

using namespace std;
 
/////////////////////////////////////////////////////////////Get time///////////////////////////////////////////////////////////////////////////////////////////////////////////
string get_time(){

    //return "";
    // Get the current time point
    auto now = std::chrono::system_clock::now();
    
    // Convert to time_t (C-style time representation)
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    
    // Get milliseconds part
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    // Format the time as "YYYYMMDD-HHMMSS.sss"
    std::tm* tm_struct = std::localtime(&now_time_t);
    char time_str[20];
    std::strftime(time_str, sizeof(time_str), "%Y%m%d-%H%M%S", tm_struct);
    
    // Construct the final timestamp string
    std::ostringstream oss;
    oss << time_str << "." << std::setw(3) << std::setfill('0') << ms.count();

    //std::cout << "Timestamp: " << oss.str() << std::endl;
    return oss.str();
}
////////////////////////////////////////////////////////////////// Record class used to print the values - not need at all  /////////////////////////////////////////////////////////////////////////////////////////////
void print(int count,string class_order_id,string instrument,int side,double price,int quantity,string status,string reason,  ofstream& outputfile){

            outputfile <<"Order" <<  count <<",";
            outputfile <<class_order_id <<",";
            outputfile << instrument << ",";
            outputfile << side  << ",";
            outputfile << price << ",";
            outputfile << quantity <<",";
            outputfile << status << ",";
            outputfile << reason << ",";
            outputfile << get_time() << endl;
            return;
            
}
///////////////////////////////////////////////////////////Order Book Buy//////////////////////////////////////////////////////////////////////////////////////////////////////

struct Order_book_buy{

    int o_side;
    string o_id;
    double o_price;
    int o_quantity;
    int o_given_id;
    
     Order_book_buy(int given_id,int side,string id ,double price,int quantity ){ // initialize the values on the buy side ofthe order book
        o_side=side;
        o_id = id;
        o_price=price;
        o_quantity=quantity;
        o_given_id = given_id;
    }
    void print_ob(){
        cout << "o_side: " << o_side << "  ";
        cout << "o_id: " << o_id << "  ";
        cout << "o_price: " << o_price << "  ";
        cout << "o_quantity: " << o_quantity << "  ";
    }
    bool operator <(Order_book_buy obj) const {     // logic of the sorting 
        return (o_price >  obj.o_price);
    }
};

////////////////////////////////////////////////////////////////////////////Order Book Sell///////////////////////////////////////////////////////////////////////////////////
struct Order_book_sell{

    int o_side;
    string o_id;
    double o_price;
    int o_quantity;
    int o_given_id;
    
     Order_book_sell(int given_id,int side,string id ,double price,int quantity ){   // initialize the values on the sell side of the order book
        o_side=side;
        o_id = id;
        o_price=price;
        o_quantity=quantity;
        o_given_id = given_id;
    }
    void print_ob(){
        cout << "o_side: " << o_side << "  ";
        cout << "o_id: " << o_id << "  ";
        cout << "o_price: " << o_price << "  ";
        cout << "o_quantity: " << o_quantity << "  ";
    }
    bool operator <(Order_book_sell obj) const {    // logic of the sorting
        return (o_price <  obj.o_price);
    }


};

//////////////////////////////////////////////////////////////////////////////////////main function/////////////////////////////////////////////////////////////////////

int main()
{   

    // Open input and output files
    ifstream inputfile;                                                      
    inputfile.open("C:\\Users\\User\\Desktop\\lseg_proj\\order.csv");     
    
    ofstream outputfile;
    outputfile.open("C:\\Users\\User\\Desktop\\lseg_proj\\execution_rep.csv");// location of the output file

    // Print the  headers in the output file
    outputfile << "client_order_id " << "," << "order_id  "<< ","  << "instrument"<< " , "  << "side"<< " , "  << "price"<< " , "  << "quantity"<< " , "  << "status"<< " , "  << "reason"<< " , "  << "transaction_time" << " , " << endl  ;
    
    string line="";


    vector <Order_book_buy> OB_buy;
    vector <Order_book_sell> OB_sell;

    // Define order books for different instruments
    vector <Order_book_buy> OB_buy_rose;
    vector <Order_book_sell> OB_sell_rose;

    vector <Order_book_buy> OB_buy_lavender;
    vector <Order_book_sell> OB_sell_lavender;

    vector <Order_book_buy> OB_buy_tulip;
    vector <Order_book_sell> OB_sell_tulip;

    vector <Order_book_buy> OB_buy_orchid;
    vector <Order_book_sell> OB_sell_orchid;

    vector <Order_book_buy> OB_buy_lotus;
    vector <Order_book_sell> OB_sell_lotus;


    int count = -1; // Variable to indicate how many lines read , lines count start from -1 which is the header and orders are starting from 0

    while(getline(inputfile,line)){  // with in this while loop we read the inputs of the order file by line by line

        //only one line is taken

        if(count<0){count++;continue;} // to ignore the first line which is just the header file of the input file
        count++;

        //make all the inputs in to separate variables 
        string class_order_id;
        string instrument;
        int side;
        int quantity;
        double price;
        string tempString;

        stringstream inputString(line);

        getline(inputString, class_order_id, ',');

        getline(inputString, instrument, ',');
        
        getline(inputString, tempString, ',');
        side = atoi(tempString.c_str());

        getline(inputString, tempString, ',');
        quantity = atoi(tempString.c_str());

        bool is_valid=true;
        string reason ;

        getline(inputString, tempString, ',');
        try {
            price = stod(tempString);
        } catch (const std::invalid_argument& e) {
            reason = "Price is not valid";
            is_valid = false;
        }


        //check whether the order is a valid order or not

       
        
        
        if(quantity>1000 || quantity<10 ){
            reason = "quantity is outside the range";
             is_valid=false;

        }else if(quantity%10!=0){
            reason = "quantity is not a multiple of 10";
             is_valid=false;

        }else if(!(price>0)){
            reason = "Price is not greater than 0";
             is_valid=false;

        }else if(instrument != "Rose" && instrument != "Lavender" && instrument != "Tulip" && instrument != "Orchid" && instrument != "Lotus" ){
            reason = "Not a valid instrument";
             is_valid=false;

        }else if(side!=1 && side!=2){
            reason = "Not a valid side";
             is_valid=false;
        }
        
        if(!is_valid){
            //outputfile << "Order" << count << " ," << class_order_id << " ," << instrument << " ," <<side << ", " << price << " ," << quantity << " ," << "Reject" << " ," << reason << " ," << get_time() << endl;
            print(count,class_order_id,instrument,side,price, quantity,"Reject",reason, outputfile);
            continue;

        }else{
        
        //iff the order is valid below parts run , here we only use two Books "OB_buy" and "OB_sell" to make the logic 
        //so we change the variables to refer the corresponding actual book
        if(instrument=="Rose"){
            OB_buy = OB_buy_rose;
            OB_sell = OB_sell_rose;
        }
        if(instrument=="Lavender"){
            OB_buy = OB_buy_lavender;
            OB_sell = OB_sell_lavender;
        }
        if(instrument=="Tulip"){
            OB_buy = OB_buy_tulip;
            OB_sell = OB_sell_tulip;
        }
        if(instrument=="Orchid"){
            OB_buy = OB_buy_orchid;
            OB_sell = OB_sell_orchid;
        }
        if(instrument=="Lotus"){
            OB_buy = OB_buy_lotus;
            OB_sell = OB_sell_lotus;
        }

        vector <Order_book_buy> vb ;
        vector <Order_book_sell> vs;

        ////////////////////////////////////////////////////////////////////////BUY side order////////////////////////////////////////////////////////////////////////////////////////

        if(side==1){    // check whether the order is in the buy side

            Order_book_buy book(count,side,class_order_id,price,quantity);
            OB_buy.push_back(book);
            sort(OB_buy.begin(), OB_buy.end());

            bool need_loop_buy = true;      // terminate the loop
            bool is_new=true;               // to stop re-printing same order on the loop
            
             //when the sell book is empty simply need to move for the next order by writing this as a new buy order in the order book
            if(OB_sell.size()==0){
                //outputfile << "Order"<< count << ", " << class_order_id << ", " << instrument << ", " <<side << ", " << price << "," << quantity << ", " << "New"<< " ," << " ," << get_time() << endl;
                print(count,class_order_id,instrument,side,price, quantity,"NEW"," ", outputfile);
                need_loop_buy=false; //if the buyer side is empty just need to put the buy order;
            }


            while(need_loop_buy){
                if(OB_sell.size()>0 && OB_buy.size()>0  ){    

                    vb=OB_buy;  //*can remove this variable shift
                    vs=OB_sell;

                    //////////////////////////////////////////logic of what to do when the current order is a buy order////////////////////////////////////////////////////////

                    if(vb[0].o_price < vs[0].o_price  ){      // can not buy the order -- seling prices are too high
                        need_loop_buy = false;
                        if(is_new){
                            //outputfile << "Order" << count << " ," << class_order_id << ", " << instrument << ", " <<side << ", " << price << " ," << quantity << " ," << "New" << " ,"<< " ," << " ," << get_time() << endl;
                            print(count,class_order_id,instrument,side,price, quantity,"NEW"," ", outputfile);
                        }
                    }
                    else if(vb[0].o_quantity <= vs[0].o_quantity){    // can completly buy the order from the top most seller // for all FILL or PFILL orders price is the selling price
                        //outputfile << "Order" << count << ", " << class_order_id << ", " << instrument << ", " <<side << ", " << vs[0].o_price << " ," << quantity << ", " << "Fill" << " ," << " ," << get_time() << endl;
                        print(count,class_order_id,instrument,side,vs[0].o_price, quantity,"Fill"," ", outputfile);
                        if(vs[0].o_quantity == vb[0].o_quantity){
                            //outputfile <<  "Order " << vs[0].o_given_id << ", " << vs[0].o_id << " ," << instrument << " ," << vs[0].o_side<< " ," << vs[0].o_price << " ," << vb[0].o_quantity << " ," << "Fill"<< " ," << " ," << get_time() << endl;
                            print(vs[0].o_given_id,vs[0].o_id,instrument, vs[0].o_side,vs[0].o_price, vb[0].o_quantity,"Fill"," ", outputfile);
                            vs.erase(vs.begin());
                        }else {
                            //outputfile <<  "Order " << vs[0].o_given_id << ", " << vs[0].o_id << ", " << instrument << ", " << vs[0].o_side<< ", " << vs[0].o_price << " ," << vb[0].o_quantity << ", " << "pFill" << " ,"<< " ," << get_time() << endl;
                            print(vs[0].o_given_id,vs[0].o_id,instrument, vs[0].o_side,vs[0].o_price, vb[0].o_quantity,"Pfill"," ", outputfile);
                            vs[0].o_quantity = vs[0].o_quantity - vb[0].o_quantity;
                        }
                        vb.erase(vb.begin());
                        need_loop_buy = false;
                    }
                    else{    // need several sellers to think about what happens with the order
                        //outputfile << "Order" << count << ", " << class_order_id << " ," << instrument << " ," <<side << ", " << vs[0].o_price << ", " << vs[0].o_quantity << ", " << "Pfill"<< " ," << ", "<< get_time() << endl;
                        print(count,class_order_id,instrument, side,vs[0].o_price, vs[0].o_quantity,"Pfill"," ", outputfile);
                        //outputfile << "Order" <<  vs[0].o_given_id << ", " << vs[0].o_id << " ," << instrument << " ," <<vs[0].o_side << " ," << vs[0].o_price << " ," << vs[0].o_quantity << ", " << "Fill"<< " ," << ", "  << get_time() << endl;
                        print(vs[0].o_given_id,vs[0].o_id ,instrument, vs[0].o_side,vs[0].o_price, vs[0].o_quantity,"Fill"," ", outputfile);
                        vb[0].o_quantity = vb[0].o_quantity - vs[0].o_quantity;
                        quantity=vb[0].o_quantity;
                        vs.erase(vs.begin());        
                        is_new = false;            
                        }
                    OB_buy=vb;
                    OB_sell=vs;
                }else{
                    need_loop_buy = false;
                }
            }
              
        ///////////////////////////////////////////////////////////////////logic of what to do when the current order is a sell order////////////////////////////////////////////////////////

        }else if(side==2){  // check whether the order is on the sell side

            Order_book_sell book(count,side,class_order_id,price,quantity);
            OB_sell.push_back(book);
            sort(OB_sell.begin(), OB_sell.end());
            bool need_loop_sell=true;
            bool is_new=true;

            if(OB_buy.size()==0){
                //outputfile << "Order" << count << " ," << class_order_id << ", " << instrument << ", " <<side << " ," << price << " ," << quantity << " ," << "New" << " ,"<< " ," << get_time() << endl;
                print(count,class_order_id ,instrument, side,price,  quantity,"New"," ", outputfile);
                need_loop_sell=false; //if the buyer side is empty just need to put the sell order
            }
            while(need_loop_sell){

                if(OB_buy.size()>0 && OB_sell.size()>0){
                    
                    vb=OB_buy;
                    vs=OB_sell;
                    
                    if(vb[0].o_price < vs[0].o_price ){      // can not buy the order -- seling pricesare too high
                        need_loop_sell=false;
                        if(is_new){
                            //outputfile << "Order" << count << " ," << class_order_id << ", " << instrument << ", " <<side << ", " << price << " ," << quantity << " ," << "New" << " ,"<< ", " << get_time() << endl;
                            print(count,class_order_id ,instrument, side,price,  quantity,"New"," ", outputfile);
                        }
                    }
                    else if(vb[0].o_quantity >= vs[0].o_quantity){    // can completly buy the order from the top most seller
                        //outputfile << "Order" << count << ", " << class_order_id << ", " << instrument << " ," <<side << " ," << vb[0].o_price << " ," << quantity << ", " << "Fill"<< " ," << ", " << get_time() << endl;
                        print(count,class_order_id ,instrument, side,vb[0].o_price,  quantity, "Fill"," ", outputfile);
                        
                        if(vb[0].o_quantity == vs[0].o_quantity){
                            //outputfile <<  "Order" << vb[0].o_given_id << "," << vb[0].o_id << " ," << instrument << " ," <<vb[0].o_side << " ," << vb[0].o_price << ", " << vb[0].o_quantity << " ," << "Fill"<< " ," << " ," << get_time() << endl;
                            print(vb[0].o_given_id, vb[0].o_id,instrument,vb[0].o_side,vb[0].o_price,  vb[0].o_quantity, "Fill"," ", outputfile);
                            vb.erase(vb.begin());
                            
                        }else{
                            //outputfile <<  "Order" << vb[0].o_given_id << " ," << vb[0].o_id << ", " << instrument << ", " <<vb[0].o_side << " ," << vb[0].o_price << " ," << vs[0].o_quantity << " ," << "Fill" << " ,"<< ", " << get_time() << endl;
                            print(vb[0].o_given_id, vb[0].o_id,instrument,vb[0].o_side,vb[0].o_price,  vs[0].o_quantity, "Fill"," ", outputfile);
                            vb[0].o_quantity = vb[0].o_quantity - vs[0].o_quantity;
                        }
                        vs.erase(vs.begin());
                        need_loop_sell=false;
                    }
                    else{    // need several sellers to think about what happens with the order
                        //outputfile need to be written
                        //outputfile << "Order" << count << ", " << class_order_id << ", " << instrument << " ," <<side << " ," << vb[0].o_price << " ," << vb[0].o_quantity << ", " << "Pfill" << " ,"<< " ," << get_time() << endl;
                        print(count, class_order_id,instrument,side,vb[0].o_price,  vb[0].o_quantity, "Pfill"," ", outputfile);
                        //outputfile << "Order" << vb[0].o_given_id << " ," << vb[0].o_id << ", " << instrument << " ," <<vb[0].o_side << " ," << vb[0].o_price << " ," << vb[0].o_quantity << " ," << "Fill"<< " ," << " ," << get_time() << endl;
                        print(vb[0].o_given_id, vb[0].o_id,instrument,vb[0].o_side,vb[0].o_price,  vb[0].o_quantity, "Fill"," ", outputfile);

                        vs[0].o_quantity = vs[0].o_quantity - vb[0].o_quantity;
                        quantity=vs[0].o_quantity;
                        vb.erase(vb.begin());
                         is_new = false;
                       
                    }
                    OB_buy=vb;
                    OB_sell=vs;
                }else{
                    need_loop_sell = false;
                }
            }
                
        }

        //rechange the dummy variable to it's actual order book representation

        if(instrument=="Rose"){
            OB_buy_rose = OB_buy;
            OB_sell_rose = OB_sell;
        }
        if(instrument=="Lavender"){
            OB_buy_lavender = OB_buy;
            OB_sell_lavender = OB_sell;
        }
        if(instrument=="Tulip"){
            OB_buy_tulip = OB_buy;
            OB_sell_tulip = OB_sell;
        }
        if(instrument=="Orchid"){
            OB_buy_orchid = OB_buy;
            OB_sell_orchid = OB_sell;
        }
        if(instrument=="Lotus"){
            OB_buy_lotus = OB_buy;
            OB_sell_lotus = OB_sell;
        }
        }
    }   
}