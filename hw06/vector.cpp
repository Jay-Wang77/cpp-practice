#include "vector.h"
#include <cmath>
#include <iterator>


namespace linalg {
auto operator<<(std::ostream& ostr, const Vector& x) -> std::ostream& {
    ostr << "[ ";
    std::copy(x.begin(), x.end(), std::ostream_iterator<float>(ostr, " "));
    ostr << "]";
    return ostr;
}
Vector::Vector(std::size_t n, float val) : data_(n, val) {

}

auto Vector::operator=(float val) -> Vector & {
    data_.assign(data_.size(), val);
    return *this;
}
auto Vector::assign(float val) -> void {
    for(auto &element : data_){
        element = val;
    }
}
auto Vector::assign(Vector v) -> void {
    data_ = v.data_;
}
auto Vector::size() const -> std::size_t {
    return data_.size();
}
auto Vector::begin() -> iterator {
    return data_.begin();
}
auto Vector::end() -> iterator {
    return data_.end();
}
/// Return an begin const_iterator to the vector
auto Vector::begin() const -> const_iterator{
    return data_.begin();
}
/// Return an end const_iterator to the vector
auto Vector::end() const -> const_iterator{
    return data_.end();
}
auto Vector::cbegin() const -> const_iterator {
    return data_.cbegin();
}
auto Vector::cend() const -> const_iterator {
    return data_.cend();
}

auto Vector::operator[](int idx) -> float & {
    if(idx < 0){
        idx = static_cast<int>(data_.size()) + idx;
    }
    if(idx >=0 && idx < static_cast<int>(data_.size())){
        return data_[idx];
    }
    throw std::out_of_range{"Index out of bounds."};
}
auto Vector::operator[](int idx) const -> const float & {
    if(idx < 0){
        idx = static_cast<int>(data_.size()) + idx;
    }
    if(idx >=0 && idx < static_cast<int>(data_.size())){
        return data_[idx];
    }
    throw std::out_of_range{"Index out of bounds."};
}
auto Vector::coeff(int idx) -> float & {
    if (idx < 0 || idx >= static_cast<int>(data_.size())) {
        throw std::out_of_range{"Index out of bounds."};
    }
    return data_[idx];
}
auto Vector::coeff(int idx) const -> const float & {
    if (idx < 0 || idx >= static_cast<int>(data_.size())) {
        throw std::out_of_range{"Index out of bounds."};
    }
    return data_[idx];
}
//v_i, every data_ add val. return re of object
auto Vector::operator+=(float val) -> Vector & {

    for(size_t i = 0; i < data_.size(); i++){
        data_[i] += val;
    }
    return *this;
}
auto Vector::operator-=(float val) -> Vector & {
    for(size_t i = 0; i < data_.size(); i++){
        data_[i] -= val;
    }
    return *this;
}
auto Vector::operator*=(float val) -> Vector & {
    for(size_t i = 0; i < data_.size(); i++){
        data_[i] *= val;
    }
    return *this;
}
auto Vector::operator/=(float val) -> Vector & {
    for(size_t i = 0; i < data_.size(); i++){
        data_[i] /= val;
    }
    return *this;
}
auto Vector::operator+=(const Vector &y) -> Vector & {
    if(data_.size() != y.data_.size()){
        throw std::invalid_argument{"different size"};
    }
    for(size_t i = 0; i<data_.size(); i++){
        data_[i] += y.data_[i];
    }
    return *this;
}
auto Vector::operator-=(const Vector &y) -> Vector & {
    if(data_.size() != y.data_.size()){
        throw std::invalid_argument{"different size"};
    }
    for(size_t i = 0; i<data_.size(); i++){
        data_[i] -= y.data_[i];
    }
    return *this;
}


auto min(const Vector &x) -> float{
    if (x.size() == 0){
        throw std::invalid_argument("empty");
    }
    return *std::min_element(x.begin(), x.end());
}
auto max(const Vector &x) -> float{
    if (x.size() == 0){
        throw std::invalid_argument("empty");
    }
    return *std::max_element(x.begin(), x.end());
}
auto argmin(const Vector &x) -> std::size_t{
    if (x.size() == 0){
        throw std::invalid_argument("empty");
    }
    return std::distance(x.begin(), std::min_element(x.begin(), x.end()));
}
auto argmax(const Vector &x) -> std::size_t{
    if (x.size() == 0){
        throw std::invalid_argument("empty");
    }
    return std::distance(x.begin(), std::max_element(x.begin(), x.end()));
}
auto non_zeros(const Vector &x) -> std::size_t {
    std::size_t count = 0;
    for (int i = 0; i< static_cast<int>(x.size());i++) {
        if (x.coeff(i) != 0.0f) {
            count++;
        }
    }
    return count;
}
auto sum(const Vector &x) -> float {
    float total = 0.0;
    if(x.size() == 0){
        total = 0.0;
    }
    for (int i = 0; i< static_cast<int>(x.size());i++) {
        total += x.coeff(i);
    }
    return total;
}
auto prod(const Vector &x) -> float{
    float produ = 1.0;
    if(x.size() == 0){
        produ = 1.0;
    }
    for (int i = 0; i< static_cast<int>(x.size());i++) {
        produ *= x.coeff(i);
    }
    return produ;
}
auto dot(const Vector &x, const Vector &y) -> float{
    if(x.size() != y.size()){
        throw std::invalid_argument("different size");
    }
    float result = 0.0;
    for (int i = 0; i< static_cast<int>(x.size()); ++i) {
        result += x.coeff(i) * y.coeff(i);
    }
    return result;
}
auto norm(const Vector &x) -> float{
    float result = 0.0;
    for (int i = 0; i< static_cast<int>(x.size()); ++i) {
        result += (x.coeff(i) * x.coeff(i));
    }
    return std::sqrt(result);
}
auto normalize(Vector &x) -> void{
    float magnitude = norm(x);
    if (magnitude != 0.0) {
        for (int i = 0; i< static_cast<int>(x.size()); ++i) {
            x.coeff(i) /= magnitude;
        }
    }
}
auto normalized(const Vector &x) -> Vector {
    Vector result = x;
    normalize(result);
    return result;
}
auto floor(const Vector &x) -> Vector {
    Vector result = x;
    for (int i = 0; i< static_cast<int>(x.size()); ++i) {
        result.coeff(i) = std::floor(result.coeff(i));
    }
    return result;
}
auto ceil(const Vector &x) -> Vector {
    Vector result = x;
    for(int i = 0; i< static_cast<int>(x.size()); ++i){
        result.coeff(i) = std::ceil(result.coeff(i));
    }
    return result;
}
auto operator+(const Vector &x) -> Vector {
    Vector result(x.size());
    for (int i = 0; i< static_cast<int>(x.size()); ++i) {
        result[i] = x[i];
    }
    return result;
}
auto operator-(const Vector &x) -> Vector {
    Vector result(x.size());
    for (int i = 0; i< static_cast<int>(x.size()); ++i) {
        result[i] = -x[i];
    }
    return result;
}
auto operator+(const Vector &x, const Vector &y) -> Vector {
    if (x.size() != y.size()) {
        throw std::invalid_argument("different size");
    }
    Vector result = x;
    for (int i = 0; i< static_cast<int>(x.size()); ++i) {
        result.coeff(i) += y.coeff(i);
    }
    return result;
}
auto operator-(const Vector &x, const Vector &y) -> Vector {
    if (x.size() != y.size()) {
        throw std::invalid_argument("different size");
    }
    Vector result = x;
    for (int i = 0; i< static_cast<int>(x.size()); ++i) {
        result.coeff(i) -= y.coeff(i);
    }
    return result;
}
auto operator+(const Vector &x, float val) -> Vector {
    Vector result = x;
    for (int i = 0; i< static_cast<int>(x.size()); ++i) {
        result.coeff(i) = result.coeff(i) + val;
    }
    return result;
}
auto operator-(const Vector &x, float val) -> Vector {
    Vector result = x;
    for (int i = 0; i< static_cast<int>(x.size()); ++i) {
        result.coeff(i) = result.coeff(i) - val;
    }
    return result;
}
auto operator*(const Vector &x, float val) -> Vector {
    Vector result = x;
    for (int i = 0; i< static_cast<int>(x.size()); ++i) {
        result.coeff(i) = result.coeff(i) * val;
    }
    return result;
}
auto operator/(const Vector &x, float val) -> Vector {
    Vector result = x;
    for (int i = 0; i< static_cast<int>(x.size()); ++i) {
        result.coeff(i) = result.coeff(i) / val;
    }
    return result;
}
auto operator+(float val, const Vector &x) -> Vector {
    Vector result = x;
    for (int i = 0; i< static_cast<int>(x.size()); ++i) {
        result.coeff(i) = result.coeff(i) + val;
    }
    return result;
}
auto operator-(float val, const Vector &x) -> Vector {
    Vector result = x;
    for (int i = 0; i< static_cast<int>(x.size()); ++i) {
        result.coeff(i) = val - result.coeff(i) ;
    }
    return result;
}
auto operator*(float val, const Vector &x) -> Vector {
    Vector result = x;
    for (int i = 0; i< static_cast<int>(x.size()); ++i) {
        result.coeff(i) = result.coeff(i) * val;
    }
    return result;
}
}

/*
 * Please Implement all functions from the header file here!#
 * Have fun! :-) And don't forget maybe you can use standard algorithms ;-)
 */
