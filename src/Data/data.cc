
#include "data.hh"

namespace CSML{

  ////////////////////////////////////////////////////////////////////////////////
  data::data(){

    this->labeltype = "UK";
    this->labelsize = 0;
    this->featuresize = 0;
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  data::~data(){}

  ////////////////////////////////////////////////////////////////////////////////
  void data::AddElement(const std::vector<double> _X, const std::vector<double> _y){

    if (this->labeltype == "UK"){
      this->labeltype = "LABELED";
      this->featuresize = _X.size();
      this->labelsize = _y.size();
    }else if (this->labeltype != "LABELED"){throw 1;}

    if (_X.size() != this->GetFeatureSize()){
      std::cout << "Feature not properly sized. Skipping" << std::endl;
      return;
    }else if (_y.size() != this->GetLabelSize()){
      std::cout << "Label not properly sized. Skipping" << std::endl;
      return;
    }
  
    this->X.push_back(_X);
    this->y.push_back(_y);
    std::vector<double> blank(this->GetLabelSize(), 0);
    this->h.push_back(blank);
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void data::AddElement(const std::vector<double> _X){

    if (this->labeltype == "UK"){
      this->labeltype = "NONE";
      this->featuresize = _X.size();
      this->labelsize = 0;
    }else if (this->labeltype != "NONE"){throw 1;}

    if (_X.size() != this->GetFeatureSize()){
      std::cout << "Feature not properly sized. Skipping" << std::endl;
      return;
    }
  
    this->X.push_back(_X);
  
  }


  ////////////////////////////////////////////////////////////////////////////////
  void data::SetHypothesis(unsigned int index, const std::vector<double> _h){

    if (this->labeltype == "UK"){throw 1;}

    if (index >= this->GetSize()){throw 1;}

    if (this->labeltype == "NONE" && this->GetLabelSize() == 0){
      this->labelsize = _h.size();
      this->h.resize(this->GetSize());
      for (unsigned int i = 0; i < this->GetSize(); ++i){
	this->h[i].resize(this->GetLabelSize());
      }
    }

    this->h[index] = _h;
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  std::string data::Save(){

    if (this->labeltype == "UK"){throw 1;}
  
    std::stringstream ss;

    ss << "featuresize:" << this->GetFeatureSize() << std::endl;
    ss << "labeltype:" << this->labeltype << std::endl;
    ss << "labelsize:" << this->GetLabelSize() << std::endl;

    if (this->labeltype == "LABELED"){
      for (unsigned int i = 0; i < this->GetSize(); ++i){
	ss << this->y[i][0] ;
	for (unsigned int l = 1; l < this->GetLabelSize(); ++l){
	  ss << "," << this->y[i][l];
	}
	ss << std::endl;
      }
    }
    for (unsigned int i = 0; i < this->GetSize(); ++i){
      ss << this->X[i][0];
      for (unsigned int f = 1; f < this->GetFeatureSize(); ++f){
	ss << "," << this->X[i][f];
	}
      ss << std::endl;
    }
    
    return ss.str();
      
    }
    
  ////////////////////////////////////////////////////////////////////////////////
  void data::Load(const std::string values){
    //fixme

    this->X.clear();
    this->y.clear();

    return;
  
  }

}
