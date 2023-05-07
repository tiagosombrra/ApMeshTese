#include "../../include/data/node_adaptive.h"

NodeAdaptive::NodeAdaptive()
    : PointAdaptive(), ga_(0), gd_(0), ha_(0), hd_(0) {}

NodeAdaptive::NodeAdaptive(double x, double y, double z)
    : PointAdaptive(x, y, z), ga_(0), gd_(0), ha_(0), hd_(0) {}

NodeAdaptive::NodeAdaptive(double x, double y, double z, unsigned long position)
    : PointAdaptive(x, y, z, position), ga_(0), gd_(0), ha_(0), hd_(0) {}

NodeAdaptive::NodeAdaptive(const PointAdaptive &point)
    : ga_(0), gd_(0), ha_(0), hd_(0) {
  this->id_ = point.GetId();
  this->x_ = point.GetX();
  this->y_ = point.GetY();
  this->z_ = point.GetZ();
}

NodeAdaptive::~NodeAdaptive() { this->elements_.clear(); }

void NodeAdaptive::InsertElement(
    const std::shared_ptr<ElementAdaptive> &element) {
  this->elements_.push_back(element);
}

unsigned int NodeAdaptive::GetNumberElements() const {
  return static_cast<unsigned int>(elements_.size());
}

std::shared_ptr<ElementAdaptive> NodeAdaptive::GetElement(
    const unsigned int position) {
  auto it = this->elements_.begin();
  advance(it, position);

  return *it;
}

void NodeAdaptive::ClearElements() { elements_.clear(); }

std::list<std::shared_ptr<ElementAdaptive>> NodeAdaptive::GetElements() const {
  return elements_;
}

void NodeAdaptive::setElements(
    const std::list<std::shared_ptr<ElementAdaptive>> &elements) {
  elements_ = elements;
}

double NodeAdaptive::GetGa() const { return ga_; }

void NodeAdaptive::SetGa(double ga) { ga_ = ga; }

double NodeAdaptive::GetGd() const { return gd_; }

void NodeAdaptive::SetGd(double gd) { gd_ = gd; }

double NodeAdaptive::GetHa() const { return ha_; }

void NodeAdaptive::SetHa(double ha) { ha_ = ha; }

double NodeAdaptive::GetHd() const { return hd_; }

void NodeAdaptive::SetHd(double hd) { hd_ = hd; }
