// written by Shengke Xue, Ph.D., Zhejiang University, 2019, email: xueshengke@zju.edu.cn
#ifndef CAFFE_WEIGHT_L2_LOSS_LAYER_HPP_
#define CAFFE_WEIGHT_L2_LOSS_LAYER_HPP_

#include <vector>

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"

#include "caffe/layers/loss_layer.hpp"

namespace caffe {

/**
 * @brief Computes the weighted Euclidean (L2) loss @f$
 *   E = \frac{1}{2N} \sum\limits_{n=1}^N \left| \left| W \circ (\hat{y}_n - y_n)
 *       \right| \right|_2^2 @f$ for real-valued regression tasks.
 *
 *
 * (Note: Caffe, and SGD in general, is certainly \b not the best way to solve
 * linear least squares problems! We use it only as an instructive example.)
 */
template <typename Dtype>
class WeightL2LossLayer : public LossLayer<Dtype> {
 public:
  explicit WeightL2LossLayer(const LayerParameter& param)
      : LossLayer<Dtype>(param), diff() {}
  virtual void Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);

  virtual inline const char* type() const { return "WeightL2Loss"; }
  /**
   * Unlike most loss layers, in the WeightL2LossLayer we can backpropagate
   * to both inputs -- override to return true and always allow force_backward.
   */
  virtual inline bool AllowForceBackward(const int bottom_index) const {
    return true;
  }

 protected:
  /// @copydoc WeightL2LossLayer
  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);

  /**
   * @brief Computes the Euclidean error gradient w.r.t. the inputs.
   *
   * Unlike other children of LossLayer, EuclideanLossLayer \b can compute
   * gradients with respect to the label inputs bottom[1] (but still only will
   * if propagate_down[1] is set, due to being produced by learnable parameters
   * or if force_backward is set). In fact, this layer is "commutative" -- the
   * result is the same regardless of the order of the two bottoms.
   *
  */
  virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);
  virtual void Backward_gpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);

  Blob<Dtype> diff;
  int num;
  int ch;
  int hei;
  int wid;
  int count;
  Blob<Dtype> W;
};

}  // namespace caffe

#endif  // CAFFE_WEIGHT_L2_LOSS_LAYER_HPP_
