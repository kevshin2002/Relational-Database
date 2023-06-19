//
//  View.hpp
//  PA1
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//


#ifndef View_h
#define View_h


#include <iostream>
#include <functional>

namespace ECE141 {

  //completely generic view, which you will subclass to show information
  class View {
  public:
    virtual         ~View() {}
    virtual bool    show(std::ostream &aStream)=0;
  };

  using ViewListener = std::function<void(View &aView)>;

}

#endif /* View_h */
