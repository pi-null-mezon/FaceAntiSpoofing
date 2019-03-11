#ifndef CUSTOMNETWORK_H
#define CUSTOMNETWORK_H

#include <dlib/dnn.h>

#define IMG_WIDTH  100
#define IMG_HEIGHT 100

#define FNUM 16

namespace dlib {

/*template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N,BN,1,tag1<SUBNET>>>;

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2,2,2,2,skip1<tag2<block<N,BN,2,tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block  = BN<con<N,3,3,1,1,relu<BN<con<N,3,3,stride,stride,SUBNET>>>>>;

template <int N, typename SUBNET> using res       = relu<residual<block,N,bn_con,SUBNET>>;
template <int N, typename SUBNET> using ares      = relu<residual<block,N,affine,SUBNET>>;
template <int N, typename SUBNET> using res_down  = relu<residual_down<block,N,bn_con,SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block,N,affine,SUBNET>>;

template <typename SUBNET> using level0 = res<32*FNUM,res_down<32*FNUM,SUBNET>>;
template <typename SUBNET> using level1 = res<16*FNUM,res_down<16*FNUM,SUBNET>>;
template <typename SUBNET> using level2 = res<8*FNUM,res_down<8*FNUM,SUBNET>>;
template <typename SUBNET> using level3 = res<4*FNUM,res_down<4*FNUM,SUBNET>>;
template <typename SUBNET> using level4 = res<2*FNUM,res_down<2*FNUM,SUBNET>>;

template <typename SUBNET> using alevel0 = ares<32*FNUM,ares_down<32*FNUM,SUBNET>>;
template <typename SUBNET> using alevel1 = ares<16*FNUM,ares_down<16*FNUM,SUBNET>>;
template <typename SUBNET> using alevel2 = ares<8*FNUM,ares_down<8*FNUM,SUBNET>>;
template <typename SUBNET> using alevel3 = ares<4*FNUM,ares_down<4*FNUM,SUBNET>>;
template <typename SUBNET> using alevel4 = ares<2*FNUM,ares_down<2*FNUM,SUBNET>>;

// training network type
using net_type = loss_multiclass_log<fc<2,avg_pool_everything<
                            
                            level4<
                            relu<bn_con<con<FNUM,7,7,2,2,
                            input_rgb_image
                            >>>>>>>;

// testing network type (replaced batch normalization with fixed affine transforms)
using anet_type = loss_multiclass_log<fc<2,avg_pool_everything<
                            
                            alevel4<
                            relu<affine<con<FNUM,7,7,2,2,
                            input_rgb_image
                            >>>>>>>;*/

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
                            relu<bn_con<con<FNUM,5,5,2,2,
                            input_rgb_image
                            >>>>>>>>>;

// testing network type (replaced batch normalization with fixed affine transforms)
using anet_type =   loss_multiclass_log<fc<2,
                            avg_pool_everything<adense2<64,FNUM,
							avg_pool<2,2,2,2,adense3<64,FNUM,
                            relu<affine<con<FNUM,5,5,2,2,
                            input_rgb_image
                            >>>>>>>>>;
}

#endif // CUSTOMNETWORK_H
