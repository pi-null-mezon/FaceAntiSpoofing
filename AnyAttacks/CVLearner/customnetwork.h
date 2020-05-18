#ifndef CUSTOMNETWORK_H
#define CUSTOMNETWORK_H

#include <dlib/dnn.h>

#define IMG_WIDTH  150
#define IMG_HEIGHT 150

#define FNUM 16

namespace dlib {

template <int N, template <typename> class BN, typename SUBNET>
using dblck  = relu<BN<con<N,3,3,1,1,relu<BN<con<4*N,1,1,1,1,SUBNET>>>>>>;

template <int N, int K, template <typename> class BN, typename SUBNET>
using dense_block2 = relu<BN<con<N,1,1,1,1, concat3<tag3,tag2,tag1,  tag3<dblck<K,BN,concat2<tag2,tag1, tag2<dblck<K,BN, tag1<SUBNET>>>>>>>>>>;

template <int N, int K, template <typename> class BN, typename SUBNET>
using dense_block3 = relu<BN<con<N,1,1,1,1, concat4<tag4,tag3,tag2,tag1, tag4<dblck<K,BN,concat3<tag3,tag2,tag1,  tag3<dblck<K,BN,concat2<tag2,tag1, tag2<dblck<K,BN, tag1<SUBNET>>>>>>>>>>>>>;

template <int N, int K, template <typename> class BN, typename SUBNET>
using dense_block4 = relu<BN<con<N,1,1,1,1, concat5<tag5,tag4,tag3,tag2,tag1,  tag5<dblck<K,BN,concat4<tag4,tag3,tag2,tag1, tag4<dblck<K,BN,concat3<tag3,tag2,tag1,  tag3<dblck<K,BN,concat2<tag2,tag1, tag2<dblck<K,BN, tag1<SUBNET>>>>>>>>>>>>>>>>;

template <int N, int K, typename SUBNET> using dense2  = dense_block2<N,K,bn_con,SUBNET>;
template <int N, int K, typename SUBNET> using dense3  = dense_block3<N,K,bn_con,SUBNET>;
template <int N, int K, typename SUBNET> using dense4  = dense_block4<N,K,bn_con,SUBNET>;

template <int N, int K, typename SUBNET> using adense2 = dense_block2<N,K,affine,SUBNET>;
template <int N, int K, typename SUBNET> using adense3 = dense_block3<N,K,affine,SUBNET>;
template <int N, int K, typename SUBNET> using adense4 = dense_block4<N,K,affine,SUBNET>;

// training network type
using net_type =    loss_multiclass_log<fc<4,
                            avg_pool_everything<dense4<64,FNUM,
                            max_pool<3,3,2,2,relu<bn_con<con<FNUM,7,7,2,2,
                            input_rgb_image>>>>>>>>;

// testing network type (replaced batch normalization with fixed affine transforms)
using anet_type =   loss_multiclass_log<fc<4,
                            avg_pool_everything<adense4<64,FNUM,
                            max_pool<3,3,2,2,relu<affine<con<FNUM,7,7,2,2,
                            input_rgb_image>>>>>>>>;

}

#endif // CUSTOMNETWORK_H
