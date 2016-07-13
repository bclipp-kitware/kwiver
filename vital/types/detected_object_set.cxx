/*ckwg +29
 * Copyright 2016 by Kitware, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither name of Kitware, Inc. nor the names of any contributors may be used
 *    to endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "detected_object_set.h"

#include <vital/vital_foreach.h>

#include <algorithm>
#include <stdexcept>

namespace kwiver {
namespace vital {

// ==================================================================
namespace {

struct less_confidence
{
  bool operator()( detected_object_sptr const& a, detected_object_sptr const& b ) const
  {
    return a->confidence() < b->confidence();
  }
};

} // end namespace


// ------------------------------------------------------------------
detected_object_set::
detected_object_set( detected_object::vector_t const& objs )
  : m_detected_objects( objs )
{
  // sort objects based on confidence
  std::sort( m_detected_objects.begin(), m_detected_objects.end(),
             less_confidence() );
}


// ------------------------------------------------------------------
size_t
detected_object_set::
size() const
{
  return m_detected_objects.size();
}


// ------------------------------------------------------------------
detected_object::vector_t
detected_object_set::
select( double threshold )
{
  detected_object::vector_t vect;

  VITAL_FOREACH( auto i, m_detected_objects )
  {
    if ( i->confidence() >= threshold )
    {
      vect.push_back( i );
    }
  }

  return vect;
}


// ------------------------------------------------------------------
const detected_object::vector_t
detected_object_set::
select( const std::string& class_name, double threshold ) const
{
  // Intermediate sortable data structure
  std::vector< std::pair< double, detected_object_sptr > > data;

  // Create a sortable list by selecting
  VITAL_FOREACH( auto i, m_detected_objects )
  {
    auto obj_type = i->type();
    if ( ! obj_type )
    {
      continue;  // Must have a type assigned
    }

    double score(0);
    try
    {
      score = obj_type->score( class_name );
    }
    catch (const std::runtime_error& )
    {
      //+ maybe log something?
      continue;
    }

    // Select those not below threshold
    if ( score >= threshold )
    {
      data.push_back( std::pair< double, detected_object_sptr >( score, i ) );
    }
  } // end foreach

  // Sort on score
  std::sort( data.begin(), data.end() );

  detected_object::vector_t vect;

  VITAL_FOREACH( auto i, data )
  {
    vect.push_back( i.second );
  }

  return vect;
}

} } // end namespace
