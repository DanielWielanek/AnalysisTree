//
// Created by eugene on 25/09/2020.
//

#ifndef ANALYSISTREE_INFRA_BRANCHREADERACTION_HPP
#define ANALYSISTREE_INFRA_BRANCHREADERACTION_HPP

#include <algorithm>

#include <AnalysisTree/BranchView.hpp>
#include <utility>

namespace AnalysisTree {

namespace BranchViewAction {

namespace Details {


}// namespace Details

class SelectFieldsAction {

  class SelectFieldsActionResultImpl : public IBranchView {
   public:
    std::vector<std::string> GetFields() const override {
      return selected_fields_;
    }
    size_t GetNumberOfChannels() const override {
      return origin_->GetNumberOfChannels();
    }
    void GetEntry(Long64_t entry) const override {
      return origin_->GetEntry(entry);
    }
    Long64_t GetEntries() const override {
      return origin_->GetEntries();
    }
    IBranchViewPtr Clone() const override {
      auto newView = std::make_shared<SelectFieldsActionResultImpl>();
      newView->origin_ = origin_->Clone();
      newView->selected_fields_ = selected_fields_;
      return newView;
    }
    IFieldPtr GetFieldPtr(std::string field_name) const override {
      return origin_->GetFieldPtr(field_name);
    }

    IBranchViewPtr origin_;
    std::vector<std::string> selected_fields_;
  };

 public:
  explicit SelectFieldsAction(std::vector<std::string> selectedFields) : selected_fields_(std::move(selectedFields)) {}
  IBranchViewPtr ApplyAction(const IBranchViewPtr& origin) {
    auto origin_fields = origin->GetFields();
    std::vector<std::string> fields_intersection;
    std::sort(origin_fields.begin(), origin_fields.end());
    std::sort(selected_fields_.begin(), selected_fields_.end());
    std::set_intersection(origin_fields.begin(), origin_fields.end(),
                          selected_fields_.begin(), selected_fields_.end(),
                          std::back_inserter(fields_intersection));

    auto action_result = std::make_shared<SelectFieldsActionResultImpl>();
    action_result->selected_fields_ = fields_intersection;
    action_result->origin_ = origin;
    return action_result;
  }

 private:
  std::vector<std::string> selected_fields_;
};


}// namespace BranchViewAction

}// namespace AnalysisTree

#endif//ANALYSISTREE_INFRA_BRANCHREADERACTION_HPP
