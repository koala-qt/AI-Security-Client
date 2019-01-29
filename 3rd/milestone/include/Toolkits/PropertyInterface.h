/**
 * @file
 *
 * @brief Header file containing definitions of toolkit property interfaces.
 */

#if !defined(Toolkits_PropertyInterface_h_INCLUDED)
#define Toolkits_PropertyInterface_h_INCLUDED

// Local headers
#include "Types.h"

// Standard headers
#include <vector>

/* === Namespace =========================================================== */

namespace NmToolkit
{

  /* === Toolkit property interface ======================================== */
  
  /* --- ImProperty class -------------------------------------------------- */
  
  /**
   * @brief Base property interface of all toolkit properties.
   *
   * All properties exposed by a toolkit must implement this interface.
   */
  class ImProperty
  {
    public:
      /** @name Constructors and destructors */ //@{
      /**
       * @brief Virtual property destructor to ensure proper destruction through interface.
       */
      virtual ~ImProperty() throw() {}

      /**
       * @brief Clone the current property.
       *
       * @return Pointer to a clone of this property.
       */
      virtual ImProperty *Clone() const = 0;
      //@}

      /** @name Data methods */ //@{
      /**
       * @brief Get number of bytes in data of property.
       *
       * @return Number of bytes in data of property.
       *
       * @see GetDataPointer()
       */
      virtual std::size_t GetDataSize() const=0;

      /**
       * @brief Get pointer to data of property.
       *
       * @return Pointer to data of property.
       *
       * @see GetDataSize()
       */
      virtual const byte_t *GetDataPointer() const=0;
      //@}
  };


  /* --- ImStringProperty class -------------------------------------------- */

  /**
   * @brief Base interface for properties containing a utf-8 string value.
   */
  class ImStringProperty : public virtual ImProperty
  {
    public:
      /** @name Get methods */ //@{
      /**
       * @brief Get the internal element of this property.
       *
       * @return An utf-8 encoded string.
       */
      virtual utf8_string_t GetValue() const = 0;

      /**
       * @brief Get the internal element of this property.
       *
       * @return An utf-8 encoded string.
       */
      virtual const utf8_string_t &GetValueRef() const = 0;
      //@}
  };


  /* --- ImUTCTimeProperty class ------------------------------------------- */

  /**
   * @brief Base interface for properties containing an utc time stamp.
   */
  class ImUTCTimeProperty : public virtual ImProperty
  {
    public:
      /** @name Get methods */ //@{
      /**
       * @brief Get the internal element of this property.
       *
       * @return UTC time stamp of property.
       */
      virtual utc_time_t GetValue() const = 0;
      //@}
  };


  /* --- ImInt32Property class --------------------------------------------- */

  /**
   * @brief Base interface for properties containing a 32-bit signed integer.
   */
  class ImInt32Property : public virtual ImProperty
  {
    public:
      /** @name Get methods */ //@{
      /**
       * @brief Get the internal element of this property.
       *
       * @return 32-bit signed integer of property.
       */
      virtual int32_t GetValue() const = 0;
      //@}
  };


  /* --- ImBooleanProperty class ------------------------------------------- */

  /**
   * @brief Base interface for properties containing a boolean value.
   */
  class ImBooleanProperty : public virtual ImProperty
  {
    public:
      /** @name Get methods */ //@{
      /**
       * @brief Get the internal element of this property.
       *
       * @return Boolean value of property.
       */
      virtual bool GetValue() const = 0;
      //@}
  };
  

  /* --- ImInt64Property class --------------------------------------------- */

  /**
   * @brief Base interface for properties containing a 64 bit integer value.
   */
  class ImInt64Property : public virtual ImProperty
  {
    public:
      /** @name Constructors and destructors*/ //@{
      /**
       * @brief Deinitilialize instance.
       */
      virtual ~ImInt64Property() throw() {}
      //@}

      /** @name Get methods */ //@{
      /**
       * @brief Get the internal element of this property.
       *
       * @return 64 bit integer value of property.
       */
      virtual int64_t GetValue() const = 0;
      //@}
  };
  

  /* --- ImDoubleProperty class ------------------------------------------- */

  /**
   * @brief Base interface for properties containing a double (floating point) value.
   */
  class ImDoubleProperty : public virtual ImProperty
  {
    public:
      /** @name Constructors and destructors*/ //@{
      /**
       * @brief Deinitilialize instance.
       */
      virtual ~ImDoubleProperty() throw() {}
      //@}

      /** @name Get methods */ //@{
      /**
       * @brief Get the internal element of this property.
       *
       * @return double value of property.
       */
      virtual double GetValue() const = 0;
      //@}
  };


  /* --- ImByteArrayProperty class ----------------------------------------- */

  /**
   * @brief Base interface for properties containing an array of byte values.
   */
  class ImByteArrayProperty : public virtual ImProperty
  {
    public:
      /** @name Get methods */ //@{
      /**
       * @brief Get the internal element of this property.
       *
       * @return A vector of bytes.
       */
      virtual const std::vector<byte_t> &GetValue() const = 0;

      /**
       * @brief Get the internal element of this property.
       *
       * @return A vector of bytes.
       */
      virtual std::vector<byte_t> &GetValue() = 0;
      //@}
  };


  /* --- ImDataProperty class ---------------------------------------------- */

  /**
   * @brief Base interface for properties containing an ImData instance.
   */
  class ImDataProperty : public virtual ImProperty
  {
    public:
      /** @name Get methods */ //@{
      /**
       * @brief Get the internal element of this property.
       *
       * @return A pointer to an ImData instance. May be NULL. 
       *   Must NOT be deallocated by caller.
       */
      virtual const class ImData* GetValue() const = 0;
      //@}
  };


  /**
   * @brief Representation of a privacy mask attached to the data.
   * 
   * Privacy mask is a grid of cells, where each cell represents
   * masking properties. The grid is being scaled to the content.
   *
   * Rows are vertical, columns are horizontal: 
   * 
   * Example:
   *
   *                  Column
   *         Row   0    1    2
   *         0
   *         1
   *         2          X
   *
   * In this example, the cell at (row,col) = (2,1) has the value X.
   * The X is the SmCellData for those coordinates, specifying the method
   * and method parameters for privacy masking.
   *
   */
  class ImPrivacyMaskProperty : public virtual ImProperty
  {
  public:
    typedef int64_t id_t;

    static const id_t UnknownId = -1;

  public:
    /**
    * The type of privacy mask
    */
    enum class cell_privacy_t : byte_t
    {
      None   = 0, //!< No privacy mask
      Solid  = 1, //!< Solid privacy mask
      Blur   = 2  //!< Blurred privacy mask
    };

    /**
    * The size of the mask
    */
    enum class mask_size_t : byte_t
    {
      Size_0  = 0,    //!< Empty grid
      Size_1  = 1,    //!< 1x1   grid
      Size_8  = 8,    //!< 8x8   grid
      Size_16 = 16,   //!< 16x16 grid
      Size_32 = 32,   //!< 32x32 grid
      Size_64 = 64    //!< 64x64 grid
    };

    /**
     * Parameters required by 
     * the solid color mask.
     */
    struct SmSolidCellParameters
    {
      byte_t R;  //!< Value of the red color channel
      byte_t G;  //!< Value of the green color channel
      byte_t B;  //!< Value of the blue color channel
    };

    /*
    * Parameters required by the
    * blurring mask.
    */
    struct SmBlurCellParameters
    {
      byte_t Level; //!< The level of blurriness from 1 to 100, where 100 is most blurry.
    };

    /**
     * Properties representing a single 
     * mask grid cell.
     */
    struct SmCellData
    {
      cell_privacy_t Type;  //!< The privacy mask type for the cell.
      bool Removable;       //!< true if the cell is removable, false if not.
      union
      {
        SmSolidCellParameters Solid;  //!< The solid cell parameters.
        SmBlurCellParameters Blur;    //!< The blurred cell parameters.
      } Parameters;                   //!< The privacy parameters for the cell.
    };
  
  public:
    /** @name Get methods */ // @{

    /**
     * @brief Get the id of the mask object. Used for fast check if the 
     *        mask should be updated.
     * @return Identifier of the mask. Changes if the underlying mask changes.
     */
    virtual int64_t GetMaskId() const = 0;

    /**
     * @brief Indicates the mask size.
     * @return Maximum supported coordinates.
     */
    virtual mask_size_t GetSize() const = 0;

    /**
     * @brief Allows per-cell properties retrieval
     * @param row: The row number of the cell you want to retrieve data from.
     * @param column: The column number of the cell you want to retrieve data from.
     * @param outData: The returned cell data properties for the specified cell.
     */
    virtual void GetCellData(byte_t row, byte_t column, SmCellData & outData) const = 0;

    /**
     * @brief Determines if a mask is empty after removing/keeping removable masks as per removableMasksLifted
     * @param removableMasksLifted: Value indicates if the privacy masks are lifted.
     */
    virtual bool IsEmpty(bool removableMasksLifted) const = 0;

    //@}
  };

}

#endif // Toolkits_PropertyInterface_h_INCLUDED
