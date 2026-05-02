#include <iostream>
#include <vector>
#include <xlnt/xlnt.hpp>


#define window_heith 600
#define window_width 800


using namespace std;
using namespace xlnt;
typedef struct VIKOR_data{
    vector <string>nams;
    vector <char>pars;
    vector <double> poids;
    vector <double> best, worst;
    vector <vector<double>> scors;
    
    vector <double> S;
    vector <double> R;

    double maS;
    double miS;

    double maR;
    double miR;

    double miQ;
    double maQ;

    vector <double> Q;
    vector<int>classing;
};

vector <vector<double>> metrax_opperation(vector <vector<double>> data_ , double fx(double x));
double oper(double x);

xlnt::workbook save_tabel( int column_start , int line_start ,
                         vector <string> paramatrs , vector <string> objects,
                         vector <vector<double>> data);
VIKOR_data VIKOR_algo(vector <vector<double>> data , vector<string>objects, vector <char> condistions , vector <double> poids);
void save_tabel(int x, int y,workbook& wb,worksheet& ws, VIKOR_data data, vector<string> par, string goalname);

int main() {
    cout<<"\n*** Ayadi ©  VIKOR program ***\n"
        <<"conditions : \n" 
        <<"\t-The table must start in the cell A1.\n"
        <<"\t-There should be no empty cells in the table.\n"
        <<"\t-Table information must be numbers except for the baselines.\n"
        <<"have fun\n\n"
        <<"load file : ";
    
    string file;
    cin>>file;

    vector <string> line;
    vector <string> row;
    vector <vector<double>>data;
   
    int line_n = 1;
    int row_n  = 2;

    workbook wb;
    wb.load(file);




    worksheet ws = wb.active_sheet();


    for(line_n ; ws.cell(line_n,1).has_value() ; line_n++){
        line.push_back( ws.cell(line_n,1).to_string());
    }
    for(row_n  ; ws.cell(1,row_n).has_value() ; row_n++){
        row.push_back( ws.cell(1,row_n).to_string());
    }
    /* read table data*/
    for(int i = 2 ; i < row_n ; i++){
        vector <double> colon;
        for(int ii = 2 ; ii < line_n ; ii++){
            colon.push_back(stod(ws.cell(ii,i).to_string()));
        }
        data.push_back(colon); 
    } 
    /***end***/

    /*chow table*/
    cout<<"\n\n***********THE TABLE*************\n\n";
    for(int i = 0 ; i < line_n-1; i++){
        cout<<line[i]<<"\t";
    }
    cout<<"\n";
    for(int i = 0 ; i < row_n-2; i++){
        cout<<row[i]<<"\t";
        for(int ii=0 ;ii< line_n-2 ; ii++){
            cout<<data[i][ii]<<"\t";
        }
        cout<<"\n";
    }
    cout<<"\n\n*********************************\n\n";
    
    vector <string> goals;
    vector <vector<char>>   goals_par;
    vector <vector<double>> poids;

    data=metrax_opperation(data,oper);

    cout<<"\nwill , thay are "<<line_n-2<<" parametar .\n";
    cout<<"How many goals are desired? : ";
    int goals_n = 0;
    cin>>goals_n;
    cout<<"\n********name it*******\n";
    for(int i = 0 ; i < goals_n ; i++){
        cout<<"goul #"<<i+1<<" : ";
        string name;
        cin>>name;
        goals.push_back(name);
    }
    cout<<"\n**********************\n";
    cout<<"conditions2 - you have \n"
        <<"\t - min.\n"
        <<"\t - max.\n"
        <<"\t - no (don't care).\n";
    for(int i = 0 ; i < goals_n ; i++){
        cout<<"*****************"<<goals[i]<<"*****************\n\n";
        vector <char> input;
        for(int ii = 0 ; ii < line_n-2 ; ii++){
            cout<<line[ii+1]<<" : ";
            int min_max;
            string couse;
            cin>>couse;
            while (couse!="min" && couse!="max" && 
                   couse!="no" && couse!="don't care" && 
                   couse!="0" && couse!="1" && couse!="2")
            {
                cout<<"\nERR: this value is not in cnoditions !\n";
                cout<<line[ii+1]<<" : ";
                cin>>couse;
            }
            if(couse == "min" || couse == "0"){
                min_max=0;
            }
            else if(couse == "max" || couse == "1"){
                min_max=1;
            }
            else if(couse == "no" || couse == "2" || couse == "don't care"){
                min_max=2;
            }
            input.push_back((char)min_max);
        }
        goals_par.push_back(input);
    }
    cout<<"\n\n****** now , pleas enter poids ******\n\n";

    for(int i = 0 ; i < goals_n ; i++){
        cout<<"***************** poids of "<<goals[i]<<" *****************\n\n";
        vector <double> input;
        for(int ii = 0 ; ii < line_n-2 ; ii++){
            cout<<line[ii+1]<<" : ";
            double min_max;
            cin>>min_max;
            input.push_back(min_max);
        }
        poids.push_back(input);
    }


    workbook save_wb;
    worksheet save_ws = save_wb.active_sheet();
    vector <vector<string>> final_save;
    vector <vector<double>> final_score;
    font _font_;/*slect _font_ preferaction*/
    _font_.size(22);
    _font_.underline();
    _font_.bold();
    _font_.name("Arial");
    font _font2_;/*slect _font_ preferaction*/
    _font2_.underline();
    _font2_.bold();
    _font2_.name("Arial");
    _font2_.color(xlnt::color::darkblue());

    border bord;/*slect boearder*/
    bord.side(border_side::bottom);
    
    int start_colon_regester = 4;
    int colon_betwine = 4;
    int y = 1;
    for(int i = 0 ; i < goals_n ; i++){
        VIKOR_data vidata=VIKOR_algo(data, row ,goals_par[i],poids[i]);
        save_tabel(1,y,save_wb,save_ws,vidata,line,goals[i]);
        y+=vidata.Q.size()+12+10;
        cout<<"\n**********  "<<goals[i]<<"  **********\n";
        cout<<"scors";
        for(int ii = 0 ; ii < vidata.scors[i].size() ; ii++){
            cout<<"\t\t";            
        }
        cout<<"S\tR\tQ\n";
        for(int i = 0 ; i < vidata.Q.size() ; i++){
            cout<<vidata.nams[i]<<"\t";
            for(int ii = 0 ; ii < vidata.scors[i].size() ; ii++){
                cout<<(float)vidata.scors[ii][i]<<"\t";
            }
            cout<<(float)vidata.S[i]<<"\t"<<(float)vidata.R[i]<<"\t"<<(float)vidata.Q[i]<<"\n";
        }
        cout<<"\n\n";
    }
    cout<<"\nchose a name of your result file : ";
    string final_file;
    cin>>final_file;
    final_file.append(".xlsx");
    save_wb.save(final_file);
    
       return 0;
}

void save_tabel(int x, int y,workbook& wb,worksheet& ws, VIKOR_data data, vector<string> par, string goalname){
    
    xlnt::font _font_;/*slect _font_ preferaction*/
    _font_.underline();
    _font_.bold();
    _font_.name("Arial");
    _font_.color(xlnt::color::black());
    _font_.size(25);
    ws.cell(x,y).value(goalname);
    ws.cell(x,y).font(_font_);
    y+=2;
    for(int i = 1 ; i < par.size() ; i++){
        ws.cell(x+i,y).value(par[i]);
    }
    y++;
    ws.cell(x,y).value("max");
    
    for(int i = 0 ; i < data.best.size() ; i++){
        ws.cell(x+i+1,y).value(data.best[i]);
    }
    y++;
    ws.cell(x,y).value("min");
    
    for(int i = 0 ; i < data.worst.size() ; i++){
        ws.cell(x+i+1,y).value(data.worst[i]);
    }
    y++;
    ws.cell(x,y).value("prameter");
    for(int i = 0 ; i < data.pars.size() ; i++){
        ws.cell(x+i+1,y).value(data.pars[i]==0?"min":data.pars[i]==1?"max":"dont care");
    }
    y++;
    ws.cell(x,y).value("poids");
    for(int i = 0 ; i < data.poids.size() ; i++){
        ws.cell(x+i+1,y).value(data.poids[i]);
    }
    y++;
    y++;
    ws.cell(x,y).value("Scors");
    ws.cell(x+2+data.scors[0].size()+0,y).value(" S ");
    ws.cell(x+2+data.scors[0].size()+1,y).value(" R ");
    ws.cell(x+2+data.scors[0].size()+2,y).value(" Q ");
    y++;
    for(int i = 0 ; i < data.scors.size() ; i++){
        ws.cell(x,y).value(data.nams[data.classing[i]]);
        for(int ii = 0  ; ii < data.scors[i].size() ; ii++){
            ws.cell(x+ii+1,y).value(data.scors[data.classing[i]][ii]);
        }
        ws.cell(x+2+data.scors[i].size()+0,y).value(data.S[data.classing[i]]);
        ws.cell(x+2+data.scors[i].size()+1,y).value(data.R[data.classing[i]]);
        ws.cell(x+2+data.scors[i].size()+2,y).value(data.Q[data.classing[i]]);
        y++;
    }
    y++;
    ws.cell(x+1+data.scors[0].size()+0,y).value("min");
    ws.cell(x+1+data.scors[0].size()+0,y+1).value("max");

    ws.cell(x+2+data.scors[0].size()+0,y).value(data.miS);
    ws.cell(x+2+data.scors[0].size()+1,y).value(data.miR);
    ws.cell(x+2+data.scors[0].size()+2,y).value(data.miQ);

    ws.cell(x+2+data.scors[0].size()+0,y+1).value(data.maS);
    ws.cell(x+2+data.scors[0].size()+1,y+1).value(data.maR);
    ws.cell(x+2+data.scors[0].size()+2,y+1).value(data.maQ);

    //y-y*=scor.size()+12
    

}






VIKOR_data VIKOR_algo(vector <vector<double>> data , vector<string>objects, vector <char> condistions , vector <double> poids){
    cout<<"VIKOR calculing ....\n\n";

    
    cout<<data.size()<<"\n";
    vector <double> best;
    vector <double> worst;

    for(int i = 0 ; i < data[0].size() ; i++ ){
        double best_=0.0;
        double worst_=0.0;
        if(condistions[i]==1){
           best_=0.0;
           worst_=0x7fffffff;
        }
        else if(condistions[i]==0){
            worst_=0.0;
            best_=0x7fffffff;
        }
        else if(condistions[i]==2){
            best_=0;
            worst_=0;
        }
        for(int ii = 0 ; ii < data.size() ; ii++){
            if(condistions[i]==1){
                if(data[ii][i]>best_){
                    best_=data[ii][i];
                }else if(data[ii][i]<worst_){
                    worst_=data[ii][i];
                }
            }
            else if(condistions[i]==0){
                if(data[ii][i]<best_){
                    best_=data[ii][i];
                }else if(data[ii][i]>worst_){
                    worst_=data[ii][i];
                }
            }
            else if(condistions[i]==2){
                break;
            }

        }
        best.push_back(best_);
        worst.push_back(worst_);
    }
    /*cout << "best : ";
    for(int i = 0 ; i < best.size() ; i++){
        cout<<best[i]<<"\t";
    }
    cout << "\nworst : ";
    for(int i = 0 ; i < best.size() ; i++){
        cout<<worst[i]<<"\t";
    }*/
    vector <vector<double>> scors;

    for(int i = 0 ; i < data.size() ; i++ ){
        vector <double> scorl;
        for(int ii = 0 ; ii < data[0].size() ; ii++){
            double scr = 0.0;
            scr=(abs(best[ii]-data[i][ii])/(best[ii]-worst[ii]))*poids[ii];
            //cout<<scr<<"\t\t";
            scorl.push_back(scr);
        }
        //cout<<"\n";
        scors.push_back(scorl);
    }

    vector <double> S;
    vector <double> R;

    double maS=0.0;
    double miS=0x7fffff;

    double maR=0.0;
    double miR=0x7fffff;

    for(int i = 0 ; i < data.size() ; i++){
        double some  = 0.0;
        double rice  = 0.0;
        for( int ii = 0 ; ii < data[0].size() ; ii++){
            some+=scors[i][ii];
            if(scors[i][ii]>rice){
                rice=scors[i][ii];
            }
        }
        
        if(some>maS){
            maS=some;
        }
        if(some<miS){
            miS=some;
        }
        if(rice>maR){
            maR=rice;
        }
        if(rice<miR){
            miR=rice;
        }
        S.push_back(some);
        R.push_back(rice);
    }
    vector <double> Q;
    double V = 0.5;

    for(int i = 0 ; i < data.size() ; i++){
        double Q_=0.0;
        Q_=V*((S[i]-miS)/(maS-miS))+((1.0-V)*((R[i]-miR)/(maR-miR)));
        Q.push_back(Q_);
    }

    double miQ=0x7fffffff, maQ=0.0;
    for(int i = 0 ; i < Q.size() ; i++){
        if(Q[i]>maQ){
            maQ=Q[i];
        }
        if(Q[i]<miQ){
            miQ=Q[i];
        }
    }
    vector<int>classing;
    
    double lastn = -1.0;
    for(int i = 0 ; i < Q.size() ; i++){
        int r = i;
        double rr = 0x7fffffff;
        for(int ii = 0 ; ii < Q.size() ; ii++){
            if(Q[ii]<rr && lastn<Q[ii]){
                rr=Q[ii];
                r=ii;
            }
        }   
        lastn=rr;
        classing.push_back(r);
    }

    VIKOR_data retdata={
        .nams=objects,
        .pars=condistions,
        .poids=poids,
        .best=best,
        .worst=worst,
        .scors=scors,
        .S=S,
        .R=R,
        .maS=maS,
        .miS=miS,
        .maR=maR,
        .miR=miR,
        .miQ=miQ,
        .maQ=maQ,
        .Q=Q,
        .classing=classing,
    };
    ///retdata.best=best;
    //retdata.worst=worst;
    
    cout<<"Calculing end .\n\n";
    return retdata;
}


vector <vector<double>> metrax_opperation(vector <vector<double>> data_ , double fx(double x)){
    for(int i = 0 ; i < data_.size() ; i++){
        for(int ii = 0 ; ii < data_[i].size() ; ii++){
            data_[i][ii]=fx(data_[i][ii]);
        }
    }
    return data_;
}



double oper(double x){
    return x*1;
}

xlnt::workbook save_tabel( int column_start , int line_start ,
                         vector <string> paramatrs , vector <string> objects,
                         vector <vector<double>> data)
{
    xlnt::workbook  wb;
    xlnt::worksheet ws = wb.active_sheet();
    for(int i = 0 ; i < paramatrs.size() ; i++){
        ws.cell(column_start+i,line_start).value(paramatrs[i]);
    }
    for(int i = 0 ; i < objects.size() ; i++){
        ws.cell(column_start,line_start+i+1).value(objects[i]);
    }
    for(int i = 0 ; i <data.size();i++){
        for(int ii = 0 ; ii <data[i].size();ii++){
            ws.cell(column_start+ii+1,line_start+i+1).value(data[i][ii]);
        }   
    }
    return wb;

}
// compile with -std=c++14 -Ixlnt/include -lxlnt
