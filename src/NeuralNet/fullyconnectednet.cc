
#include "fullyconnectednet.hh"

namespace CSML{
  
  ////////////////////////////////////////////////////////////////////////////////
  fullyconnectednet::fullyconnectednet(const std::vector<unsigned int> nodes_per_layer){

    this->learning_rate = 1;

    // Add the nodes
    this->SetShape(nodes_per_layer);

    // Add synapses
    for (unsigned int i = 1; i < this->nodes.size(); ++i){
      for (unsigned int n = 0; n < this->nodes[i].size(); ++ n){
	this->AddSynapse(this->bias_node, this->nodes[i][n]);// Synapse with the bias node
	for (unsigned int s = 0; s < this->nodes[i-1].size(); ++s){
	  this->AddSynapse(this->nodes[i-1][s], this->nodes[i][n]);// Add the synapses to the previous layer
	}
      }
    }

  }

  ////////////////////////////////////////////////////////////////////////////////
  fullyconnectednet::~fullyconnectednet(){}

  ////////////////////////////////////////////////////////////////////////////////
  void fullyconnectednet::Learn(data* _X){

    for (unsigned int i = 0; i < this->itterations; ++i){
      for (unsigned int f = 0; f < _X->GetSize(); ++f){
	this->Reset();
	this->Propogate(*_X->GetElement(f));
	this->BackPropogate(*_X->GetLabel(f));
	this->Update();
      }
    }
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void fullyconnectednet::Predict(data* _X){
    
    for (unsigned int i = 0; i < _X->GetSize(); ++i){
	this->Reset();
	this->Propogate(*_X->GetElement(i));
	_X->SetHypothesis(i, this->Output());
    }

  }

  ////////////////////////////////////////////////////////////////////////////////
  void fullyconnectednet::SetParam(const std::string param, const double value){
    if (param == "learningrate" && value > 0){
      this->learning_rate = value;
      for (unsigned int i = 0; i < this->nodes.size(); ++i){
	for (unsigned int d = 0; d < this->nodes[i].size(); ++d){
	  this->nodes[i][d]->SetLearningRate(value);
	}
      }
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  void fullyconnectednet::SetParam(const std::string param, const int value){
    if (param == "itterations" && value > 0){
      this->itterations = value;
    }
  }
  
}
