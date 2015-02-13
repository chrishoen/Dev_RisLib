/* CPPDOC_BEGIN_EXCLUDE  */
//******************************************************************************
//******************************************************************************
//
// FILE:        Callback.h
// DESCRIPTION: See GotW 83 for an explanation of the whys and wherefores.
// These callback classes encapsulate a function call so users can pass and
// store function pointers.
//
//******************************************************************************
//
// PROPRIETARY INFORMATION:  THE INFORMATION DISCLOSED IN THIS DOCUMENT WAS
// EITHER ORIGINATED BY, OR LICENSED FROM 3rd PARTIES BY, HARRIS CORPORATION /
// RF COMMUNICATIONS DIVISION. HARRIS ORIGINATED PROPRIETARY INFORMATION IS TO
// BE USED ONLY FOR THE SPECIFIC PURPOSES FOR WHICH IT WAS SUPPLIED AND IS NOT
// TO BE DISCLOSED TO OTHERS OR REPRODUCED WITHOUT THE PRIOR WRITTEN CONSENT
// OF HARRIS CORPORATION / RF COMMUNICATIONS DIVISION. LICENSED 3rd PARTY
// MATERIALS DISTRIBUTED HEREIN SHALL BE USED ONLY TO THE EXTENT PERMITTED BY
// THE APPLICABLE LICENSE.
//
//******************************************************************************
//
// Government Use Rights:
//
//           (Applicable only for source code delivered under U. S.
//           Government contracts)
//
//                           RESTRICTED RIGHTS LEGEND
//           Use, duplication, or disclosure is subject to restrictions
//           stated in the Government's contract with Harris Corporation,
//           RF Communications Division. The applicable contract number is
//           indicated on the media containing this software. As a minimum,
//           the Government has restricted rights in the software as
//           defined in DFARS 252.227-7013.
//
// Commercial Use Rights:
//
//           (Applicable only for source code procured under contracts other
//           than with the U. S. Government)
//
//                           TRADE SECRET
//           Contains proprietary information of Harris Corporation.
//
// Copyright:
//           Protected as an unpublished copyright work,
//                    (c) Harris Corporation
//           First fixed in 2004, all rights reserved.
//
//******************************************************************************
//******************************************************************************
/* CPPDOC_END_EXCLUDE */

#ifndef __CALLBACK_H_
#define __CALLBACK_H_

#include <new>

namespace Utils
{
   class CallbackBase
   {
   public:
     virtual void operator()() const { };
//   virtual ~CallbackBase() = 0;
   };

   template<typename T>
   class Callback : public CallbackBase
   {
   public:
     typedef void (T::*F)();

     Callback( T& t, F f ) : t_(&t), f_(f) { }

     void operator()() const { (t_->*f_)(); }

   private:
     T* t_;
     F  f_;

     Callback();
   };

   template<typename T>
   Callback<T> make_callback( T& t, void (T::*f) () )
   {
     return Callback<T>( t, f );
   }

   template<typename T>
   Callback<T> * dup_callback( Callback<T> & cbt )
   {
      return new(std::nothrow) Callback<T>(cbt);      
   }
   
   template<typename T, typename P1>
   class CallbackP1 : public CallbackBase
   {
   public:
     typedef void (T::*F)(P1);

     CallbackP1( T& t, F f, P1 p1 ) : t_(&t), f_(f), p1_(p1) { }
     CallbackP1( T& t, F f ) : t_(&t), f_(f) { }

     void operator()() const { (t_->*f_)(p1_); }
     void operator()( P1 p1 ) const { (t_->*f_)(p1); }

     void SetP1( P1 p1 ) { p1_ = p1; }

   private:
     T* t_;
     F  f_;
     P1 p1_;
     
     CallbackP1();
   };

   template<typename T, typename P1>
   CallbackP1<T, P1> make_callback( T& t, void (T::*f) (P1), P1 &p1 )
   {
     return CallbackP1<T, P1>( t, f, p1 );
   }

   template<typename T, typename P1>
   CallbackP1<T, P1> * dup_callback( CallbackP1<T, P1> & cbt )
   {
      return new(std::nothrow) CallbackP1<T, P1>(cbt);      
   }
   
   template<typename T, typename P1, typename P2>
   class CallbackP2 : public CallbackBase
   {
   public:
     typedef void (T::*F)(P1, P2);

     CallbackP2( T& t, F f, P1 p1, P2 p2) : t_(&t), f_(f), p1_(p1), p2_(p2) { }
     CallbackP2( T& t, F f ) : t_(&t), f_(f) { }

     void operator()() const { (t_->*f_)(p1_, p2_); }
     void operator()( P1 p1, P2 p2 ) const { (t_->*f_)(p1, p2); }
     void operator()( P1 p1 ) const { (t_->*f_)(p1, p2_); } // njs - added for default paramaters 

     void SetP1( P1 p1 ) { p1_ = p1; }
     void SetP2( P2 p2 ) { p2_ = p2; }

   private:
     T* t_;
     F  f_;
     P1 p1_;
     P2 p2_;
     
     CallbackP2();
   };

   template<typename T, typename P1, typename P2>
   CallbackP2<T, P1, P2> make_callback( T& t, void (T::*f) (P1, P2), P1 &p1, P2 &p2 )
   {
     return CallbackP2<T, P1, P2>( t, f, p1, p2 );
   }

   template<typename T, typename P1, typename P2>
   CallbackP2<T, P1, P2> * dup_callback( CallbackP2<T, P1, P2> & cbt )
   {
      return new(std::nothrow) CallbackP2<T, P1, P2>(cbt);      
   }
   
   template<typename T, typename P1, typename P2, typename P3>
   class CallbackP3 : public CallbackBase
   {
   public:
     typedef void (T::*F)(P1, P2, P3);

     CallbackP3( T& t, F f, P1 p1, P2 p2, P3 p3) : t_(&t), f_(f), p1_(p1),
                                                   p2_(p2), p3_(p3){ }
     CallbackP3( T& t, F f ) : t_(&t), f_(f) { }

     void operator()() const { (t_->*f_)(p1_, p2_, p3_); }
     void operator()( P1 p1, P2 p2, P3 p3 ) const { (t_->*f_)(p1, p2, p3); }
     void operator()( P1 p1, P2 p2 ) const { (t_->*f_)(p1, p2, p3_); } // njs - added for default paramaters 
     void operator()( P1 p1 ) const { (t_->*f_)(p1, p2_, p3_); } // njs - added for default paramaters 

     void SetP1( P1 p1 ) { p1_ = p1; }
     void SetP2( P2 p2 ) { p2_ = p2; }
     void SetP3( P3 p3 ) { p3_ = p3; }

   private:
     T* t_;
     F  f_;
     P1 p1_;
     P2 p2_;
     P3 p3_;
     
     CallbackP3();
   };

   template<typename T, typename P1, typename P2, typename P3>
   CallbackP3<T, P1, P2, P3> make_callback( T& t, void (T::*f) (P1, P2, P3),
                                        P1 &p1, P2 &p2, P3 &p3 )
   {
     return CallbackP3<T, P1, P2, P3>( t, f, p1, p2, p3 );
   }
   
   template<typename T, typename P1, typename P2, typename P3>
   CallbackP3<T, P1, P2, P3> * dup_callback( CallbackP3<T, P1, P2, P3> & cbt )
   {
      return new(std::nothrow) CallbackP3<T, P1, P2, P3>(cbt);      
   }

   
   template<typename T, typename P1, typename P2, typename P3, typename P4>
   class CallbackP4 : public CallbackBase
   {
   public:
     typedef void (T::*F)(P1, P2, P3, P4);

     CallbackP4( T& t, F f, P1 p1, P2 p2, P3 p3, P4 p4) : t_(&t), f_(f), p1_(p1),
                                                   p2_(p2), p3_(p3), p4_(p4){ }
     CallbackP4( T& t, F f ) : t_(&t), f_(f) { }

     void operator()() const { (t_->*f_)(p1_, p2_, p3_, p4_); }
     void operator()( P1 p1, P2 p2, P3 p3, P4 p4 ) const { (t_->*f_)(p1, p2, p3, p4); }
     void operator()( P1 p1, P2 p2, P3 p3 ) const { (t_->*f_)(p1, p2, p3, p4_); }
     void operator()( P1 p1, P2 p2 ) const { (t_->*f_)(p1, p2, p3_, p4_); } // njs - added for default paramaters 
     void operator()( P1 p1 ) const { (t_->*f_)(p1, p2_, p3_, p4_); } // njs - added for default paramaters 

     void SetP1( P1 p1 ) { p1_ = p1; }
     void SetP2( P2 p2 ) { p2_ = p2; }
     void SetP3( P3 p3 ) { p3_ = p3; }
     void SetP4( P4 p4 ) { p4_ = p4; }

   private:
     T* t_;
     F  f_;
     P1 p1_;
     P2 p2_;
     P3 p3_;
     P4 p4_;
     
     CallbackP4();
   };

   template<typename T, typename P1, typename P2, typename P3, typename P4>
   CallbackP4<T, P1, P2, P3, P4> make_callback( T& t, void (T::*f) (P1, P2, P3, P4),
                                        P1 &p1, P2 &p2, P3 &p3, P4 &p4 )
   {
     return CallbackP4<T, P1, P2, P3, P4>( t, f, p1, p2, p3, p4 );
   }
   
   template<typename T, typename P1, typename P2, typename P3, typename P4>
   CallbackP4<T, P1, P2, P3, P4> * dup_callback( CallbackP4<T, P1, P2, P3, P4> & cbt )
   {
      return new(std::nothrow) CallbackP4<T, P1, P2, P3, P4>(cbt);      
   }
   
   

   
   template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5>
   class CallbackP5 : public CallbackBase
   {
   public:
     typedef void (T::*F)(P1, P2, P3, P4, P5);

     CallbackP5( T& t, F f, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) : t_(&t), f_(f), p1_(p1),
                                                   p2_(p2), p3_(p3), p4_(p4), p5_(p5){ }
     CallbackP5( T& t, F f ) : t_(&t), f_(f) { }

     void operator()() const { (t_->*f_)(p1_, p2_, p3_, p4_, p5_); }
     void operator()( P1 p1, P2 p2, P3 p3, P4 p4, P5 p5 ) const { (t_->*f_)(p1, p2, p3, p4, p5); }
     void operator()( P1 p1, P2 p2, P3 p3, P4 p4 ) const { (t_->*f_)(p1, p2, p3, p4, p5_); }
     void operator()( P1 p1, P2 p2, P3 p3 ) const { (t_->*f_)(p1, p2, p3, p4_, p5_); }
     void operator()( P1 p1, P2 p2 ) const { (t_->*f_)(p1, p2, p3_, p4_, p5_); } // njs - added for default paramaters 
     void operator()( P1 p1 ) const { (t_->*f_)(p1, p2_, p3_, p4_, p5_); } // njs - added for default paramaters 

     void SetP1( P1 p1 ) { p1_ = p1; }
     void SetP2( P2 p2 ) { p2_ = p2; }
     void SetP3( P3 p3 ) { p3_ = p3; }
     void SetP4( P4 p4 ) { p4_ = p4; }
     void SetP5( P5 p5 ) { p5_ = p5; }

   private:
     T* t_;
     F  f_;
     P1 p1_;
     P2 p2_;
     P3 p3_;
     P4 p4_;
     P5 p5_;
      
     CallbackP5();
   };

   template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5>
   CallbackP5<T, P1, P2, P3, P4, P5> make_callback( T& t, void (T::*f) (P1, P2, P3, P4, P5),
                                        P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5 )
   {
     return CallbackP5<T, P1, P2, P3, P4, P5>( t, f, p1, p2, p3, p4, p5 );
   }
   
   template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5>
   CallbackP5<T, P1, P2, P3, P4, P5> * dup_callback( CallbackP5<T, P1, P2, P3, P4, P5> & cbt )
   {
      return new(std::nothrow) CallbackP5<T, P1, P2, P3, P4, P5>(cbt);      
   }
   
}
#endif

