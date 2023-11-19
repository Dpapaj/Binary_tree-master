/*!
 * \file       data.h
 * \author     Dulik
 * \date       9. 3. 2015
 * \brief      Declaration of the data module.
 * **********************************************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */
#ifndef DATA_H_
#define DATA_H_

/*! \defgroup data Data
 *
 *  \brief Base data structure that is used in project.
 *
 *  \{
 */
/* Includes --------------------------------------------------------------------------------------*/
#include <stdbool.h>

/* Exported macros -------------------------------------------------------------------------------*/
/* Exported types --------------------------------------------------------------------------------*/
/*!
 * Defines one collection of personal data
 */
typedef struct {
  /*! Name of person. */
  char name[255];

  /*! Age of person. */
  double age;

  /*! Weight of person. */
  double weight;

  /*! Height of person. */
  double height;
} Data_t;

/* Exported constants ----------------------------------------------------------------------------*/
/* Exported variables ----------------------------------------------------------------------------*/
/* Exported functions declarations ---------------------------------------------------------------*/
/*! Compares two structures and returns which one is first.
 *
 * \param[in] d1  LHS value to compare.
 * \param[in] d2  RHS value to compare.
 *
 * \return Returns zero if LHS is equal to RHS, less than zero if LHS is smaller
 * than RHS, more than zero otherwise.
 */
int Data_Cmp(const Data_t *d1, const Data_t *d2);

/*! Reads new values to the \a data.
 *
 * \param[out] data Pointer to the memory where loaded values will be stored.
 *
 * \return Returns true on success, false otherwise.
 */
bool Data_Get(Data_t *data);

/*! Prints the \a data values to the stdout in predefined format.
 *
 * \param[in] data  Structure to print.
 */
void Data_Print(Data_t *data);

#endif /* DATA_H_ */

/*! \} */