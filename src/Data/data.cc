
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
      this->labeltype = "MULTI";
      this->featuresize = _X.size();
      this->labelsize = _y.size();
    }else if (this->labeltype != "MULTI"){throw 2;}

    if (_X.size() != this->featuresize){
      std::cout << "Feature not properly sized. Skipping" << std::endl;
      return;
    }else if (_y.size() != this->labelsize){
      std::cout << "Label not properly sized. Skipping" << std::endl;
      return;
    }
  
    this->X.push_back(_X);
    this->y2.push_back(_y);

  }


  ////////////////////////////////////////////////////////////////////////////////
  void data::AddElement(const std::vector<double> _X, const double _y){

    if (this->labeltype == "UK"){
      this->labeltype = "SINGLE";
      this->featuresize = _X.size();
      this->labelsize = 0;
    }else if (this->labeltype != "SINGLE"){throw 2;}

    if (_X.size() != this->featuresize){
      std::cout << "Feature not properly sized. Skipping" << std::endl;
      return;
    }
  
    this->X.push_back(_X);
    this->y1.push_back(_y);

  }

  ////////////////////////////////////////////////////////////////////////////////
  void data::AddElement(const std::vector<double> _X){

    if (this->labeltype == "UK"){
      this->labeltype = "NONE";
      this->featuresize = _X.size();
      this->labelsize = 0;
    }else if (this->labeltype != "NONE"){throw 2;}

    if (_X.size() != this->featuresize){
      std::cout << "Feature not properly sized. Skipping" << std::endl;
      return;
    }
  
    this->X.push_back(_X);
  
  }


  ////////////////////////////////////////////////////////////////////////////////
  void data::AddElements(const std::vector< std::vector<double> > _X, const std::vector< std::vector<double> > _y){

    for (unsigned int i = 0; i < _X.size(); ++i){
      this->AddElement(_X[i], _y[i]);
    }

  }

  ////////////////////////////////////////////////////////////////////////////////
  void data::AddElements(const std::vector< std::vector<double> > _X, const std::vector<double> _y){

    for (unsigned int i = 0; i < _X.size(); ++i){
      this->AddElement(_X[i], _y[i]);
    }

  }

  ////////////////////////////////////////////////////////////////////////////////
  void data::AddElements(const std::vector< std::vector<double> > _X){

    for (unsigned int i = 0; i < _X.size(); ++i){
      this->AddElement(_X[i]);
    }

  }

  ////////////////////////////////////////////////////////////////////////////////
  void data::SetHypothesis(const std::vector< std::vector<double> > _h){

    if (!(this->labeltype == "NONE" || this->labeltype == "MULTI")){throw 2;}

    if (_h.size() != this->X.size()){throw 2;}

    if (this->labeltype == "NONE" && this->labelsize == 0){this->labelsize = _h[0].size();}

    this->h2.resize(this->X.size());

    for (unsigned int i = 0; i < _h.size(); ++i){
      if (_h[i].size() != this->labelsize){throw 2;}
      this->h2[i] = _h[i];
    }
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void data::SetHypothesis(const std::vector<double> _h){

    if (!(this->labeltype == "NONE" || this->labeltype == "SINGLE")){throw 2;}

    if (_h.size() != this->X.size()){throw 2;}

    this->h1 = _h;
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  double data::TotalLoss(){

    std::vector<double> featureloss = this->FeatureLoss();
    double loss = 0;

    for (unsigned int i = 0; i < featureloss.size(); ++i){
      loss += featureloss[i];
    }

    return loss;
        
  }

  ////////////////////////////////////////////////////////////////////////////////
  std::vector<double> data::FeatureLoss(){

    std::vector<double> loss(this->X.size());
    
    if (this->labeltype == "SINGLE"){
      for (unsigned int i = 0; i < this->y1.size(); ++i){
	loss[i] = pow(this->y1[i] - this->h1[i],2);
      }
    }else if (this->labeltype == "MULTI"){
      for (unsigned int i = 0; i < this->y2.size(); ++i){
	loss[i] = 0;
	for (unsigned int l = 0; l < this->labelsize; ++l){
	  loss[i] += pow(this->y2[i][l] - this->h2[i][l],2);
	}
      }
    }
    
    return loss;
    
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  void data::NormalizeData(const std::string normtype){

    double norm = 0;
  
    if (normtype == "L2"){
      for (unsigned int i = 0; i < this->X.size(); ++i){
	norm = 0;
	for (unsigned int f = 0; f < this->featuresize; ++f){
	  norm += this->X[i][f]*this->X[i][f];
	}
	norm = sqrt(norm);
	for (unsigned int f = 0; f < this->featuresize; ++f){
	  this->X[i][f] /= norm;
	}
      }
    }else{
      throw 2;
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  void data::ScaleData(const double scale){
  
    for (unsigned int i = 0; i < this->X.size(); ++i){
      for (unsigned int f = 0; f < this->featuresize; ++f){
	this->X[i][f] *= scale;
      }
    }
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  void data::ProcessData(std::vector<double> (*f)(std::vector<double>)){

    for (unsigned int i = 0; i < this->X.size(); ++i){
      this->X[i] = (*f)(this->X[i]);
    }
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  double data::MeanLabel(const int l){

    double total = 0;
  
    if (this->labeltype == "MULTI"){
      for (unsigned int i = 0; i < this->y2.size(); ++i){
	total += this->y2[i][l];
      }
      return total/this->y2.size();
    }else if (this->labeltype == "SINGLE"){
      for (unsigned int i = 0; i < this->y1.size(); ++i){
	total += this->y1[i];
      }
      return total/this->y1.size();
    }else{
      throw 2;
    }
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  double data::STDLabel(const int l){

    double mean = this->MeanLabel(l);

    double variance = 0;

    if (this->labeltype == "MULTI"){
      for (unsigned int i = 0; i < this->y2.size(); ++i){
	variance += pow(this->y2[i][l] - mean ,2);
      }
      return sqrt(variance/this->y2.size());
    }else if (this->labeltype == "SINGLE"){
      for (unsigned int i = 0; i < this->y1.size(); ++i){
	variance += pow(this->y1[i] - mean ,2);
      }
      return sqrt(variance/this->y1.size());    
    }else{
      throw 2;
    }
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  std::vector< std::vector<double> >* data::GetFeatures(){

    return &this->X;
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  std::vector< std::vector<double> >* data::GetLabelsVV(){

    if (this->labeltype != "MULTI"){throw 2;}
  
    return &this->y2;
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  std::vector<double>* data::GetLabelsV(){

    if (this->labeltype != "SINGLE"){throw 2;}
  
    return &this->y1;
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  std::string data::GetLabelType(){

    return this->labeltype;
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  std::string data::Save(){

    if (this->labeltype == "UK"){throw 1;}
  
    std::stringstream ss;

    ss << "featuresize:" << this->featuresize << std::endl;
    ss << "labeltype:" << this->labeltype << std::endl;
    ss << "labelsize:" << this->labelsize << std::endl;

    if (this->labeltype == "MULTI"){
      for (unsigned int i = 0; i < this->y2.size(); ++i){
	ss << this->y2[i][0] ;
	for (unsigned int l = 1; l < this->labelsize; ++l){
	  ss << "," << this->y2[i][l];
	}
	ss << std::endl;
      }
    }else if (this->labeltype == "SINGLE"){
      ss << this->y1[0] ;
      for (unsigned int i = 1; i < this->y1.size(); ++i){
	ss << "," << this->y1[i];
      }
      ss << std::endl;
    }

    for (unsigned int i = 0; i < this->X.size(); ++i){
      ss << this->X[i][0];
      for (unsigned int f = 1; f < this->featuresize; ++f){
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
    this->y1.clear();
    this->y2.clear();

    return;
  
  }

}
