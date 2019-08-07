#ifndef CUSTOMNETWORK_H
#define CUSTOMNETWORK_H

#include <dlib/dnn.h>

#define IMG_WIDTH  100
#define IMG_HEIGHT 100

#define FNUM 8

namespace dlib {

template <int N, template <typename> class BN, typename SUBNET>
using block  = relu<BN<con<N,3,3,1,1,relu<BN<con<4*N,1,1,1,1,SUBNET>>>>>>;

template <int N, int K, template <typename> class BN, typename SUBNET>
using dense_block2 = relu<BN<con<N,1,1,1,1, concat3<tag3,tag2,tag1,  tag3<block<K,BN,concat2<tag2,tag1, tag2<block<K,BN, tag1<SUBNET>>>>>>>>>>;

template <int N, int K, template <typename> class BN, typename SUBNET>
using dense_block3 = relu<BN<con<N,1,1,1,1, concat4<tag4,tag3,tag2,tag1, tag4<block<K,BN,concat3<tag3,tag2,tag1,  tag3<block<K,BN,concat2<tag2,tag1, tag2<block<K,BN, tag1<SUBNET>>>>>>>>>>>>>;

template <int N, int K, template <typename> class BN, typename SUBNET>
using dense_block4 = relu<BN<con<N,1,1,1,1, concat5<tag5,tag4,tag3,tag2,tag1,  tag5<block<K,BN,concat4<tag4,tag3,tag2,tag1, tag4<block<K,BN,concat3<tag3,tag2,tag1,  tag3<block<K,BN,concat2<tag2,tag1, tag2<block<K,BN, tag1<SUBNET>>>>>>>>>>>>>>>>;

template <int N, int K, typename SUBNET> using dense2  = dense_block2<N,K,bn_con,SUBNET>;
template <int N, int K, typename SUBNET> using dense3  = dense_block3<N,K,bn_con,SUBNET>;
template <int N, int K, typename SUBNET> using dense4  = dense_block4<N,K,bn_con,SUBNET>;

template <int N, int K, typename SUBNET> using adense2 = dense_block2<N,K,affine,SUBNET>;
template <int N, int K, typename SUBNET> using adense3 = dense_block3<N,K,affine,SUBNET>;
template <int N, int K, typename SUBNET> using adense4 = dense_block4<N,K,affine,SUBNET>;

// training network type
using net_type =    loss_multiclass_log<fc<2,
                            avg_pool_everything<dense2<64,FNUM,
                            avg_pool<2,2,2,2,dense3<64,FNUM,
                            avg_pool<2,2,2,2,dense3<64,FNUM,
                            relu<bn_con<con<FNUM,5,5,2,2,
                            input_rgb_image
                            >>>>>>>>>>>;

// testing network type (replaced batch normalization with fixed affine transforms)
using anet_type =   loss_multiclass_log<fc<2,
                            avg_pool_everything<adense2<64,FNUM,
                            avg_pool<2,2,2,2,adense3<64,FNUM,
                            avg_pool<2,2,2,2,adense3<64,FNUM,
                            relu<affine<con<FNUM,5,5,2,2,
                            input_rgb_image
                            >>>>>>>>>>>;

}

#endif // CUSTOMNETWORK_H
