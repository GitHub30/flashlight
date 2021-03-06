/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
/*******************************************************
 * Copyright (c) 2017, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#pragma once

#include <flashlight/autograd/Variable.h>
#include <flashlight/common/Serialization.h>

#include <string>
#include <vector>

namespace fl {

/**
 * A abstract computation unit capable of forward computation. Also
 * contains a collection of parameters that can be mutated, and will be
 * serialized and deserialized with the module.
 */
class Module {
 private:
  /**
   * Serialize the module's parameters.
   */
  FL_SAVE_LOAD(params_, train_)

 protected:
  /**
   * Parameters of module, represented as a collection of `Variable`, whose
   * ordering is based on the implementation of the respective module.
   */
  std::vector<Variable> params_;

  /**
   * A flag specifying whether or not the module is in `train` mode. If
   * `Module::train()` is called, it will be set to true, and if
   * `Module::eval()` is called, it will be set to false.
   */
  bool train_ = true;

  /**
   * An empty module constructor, which creates a module with no parameters.
   */
  Module();

  /**
   * Constructs a module given its parameters.
   *
   * @param params a vector of `Variable` which will replace `params_`
   */
  explicit Module(const std::vector<Variable>& params);

 public:
  /**
   * Gets the parameters of the module.
   *
   * @return the modules parameters as a vector of `Variable`
   */
  std::vector<Variable> params() const;

  /**
   * Switches the module to training mode. Changes all parameters so that
   * gradient calculation will be enabled for any calls to `forward`.
   */
  virtual void train();

  /**
   * Switches the module to evalulation mode. Changes all parameters so that
   * gradient calculation will be disabled for any calls to `forward`.
   */
  virtual void eval();

  /**
   * Returns a module parameter given a particular position.
   *
   * @param position the index of the requested parameter in `params_`
   * @return a `Variable` tensor for the parameter at the requested position
   */
  Variable param(int position) const;

  /**
   * Sets a parameter at a specified position with a new, given one.
   *
   * If the specified position is not valid (it is negative or greater than
   * ``params_.size() - 1``), then an error will be thrown. A new parameter
   * will not be created at a specified index if out of bounds.
   *
   * @param var the new replacement `Variable`
   * @param position The index of the parameter which will be replaced in
   * `params_`
   */
  virtual void setParams(const Variable& var, int position);

  /**
   * Clears references to gradient Variables for all parameters in the module.
   */
  void zeroGrad();

  /**
   * Performs forward computation for the module, given some input.
   *
   * @param input the value on which to compute forward computation for the
   * module.
   * @return a `Variable` tensor containing the result of the forward
   * computation
   */
  virtual Variable forward(const Variable& input) = 0;

  /**
   * Overload for forward computation for the module.
   *
   * @param input the value on which to compute forward computation for the
   * module.
   * @return a `Variable` tensor containing the result of the forward
   * computation
   */
  Variable operator()(const Variable& input);

  /**
   * Generates a stringified representation of the module.
   *
   * @return a string containing the module label
   */
  virtual std::string prettyString() const = 0;

  virtual ~Module() = default;
};

} // namespace fl
