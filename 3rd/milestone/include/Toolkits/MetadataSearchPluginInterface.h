/**
 * @file
 *
 * @brief Header file defining the Metadata Search Plug-in Interface.
 */

#ifndef Metadata_Search_Plugin_Interface_h_INCLUDED
#define Metadata_Search_Plugin_Interface_h_INCLUDED

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/* === Macros ============================================================== */

#ifdef MSPI_IMPORT
  #define MSPI_API typedef
  #define MSPI_FUNC(func)((*func ## _t))
#else
  #ifdef _WIN32
    #define MSPI_API __declspec(dllexport)
  #else
    #define MSPI_API __attribute__((visibility("default")))
  #endif
  #define MSPI_FUNC(func)(func)
#endif

/**
 * @def MSPI_API
 *
 * @brief Macro to set visibility of metadata search plug-in functions.
 */

/**
 * @def MSPI_FUNC
 *
 * @brief Macro to modify function names of metadata search plug-in functions.
 */


/* === Tutorial documentation ============================================== */

/**
 * @mainpage Tutorial
 *
 * @section introduction Introduction
 *
 * Since metadata in general can be any kind of data, it is not possible to
 * make a generic and still efficient search engine that works great in all
 * cases. On the other hand, if you do know the metadata to index and
 * also what kind of queries you would like to execute, then you can also often
 * use that information to implement a very efficient search engine. This
 * however of course requires that you will be able to write your own search
 * engine plug-in taking full control of indexing and making queries.
 *
 * In this tutorial it will be described how to write your own custom
 * <em>metadata search plug-in</em> for indexing and querying metadata. As an
 * example, you could write a custom plug-in that would just act as a proxy
 * to a third party database server. It could e.g. be using a SQL server or
 * maybe use Elasticsearch for distributed full text searching. A simple search
 * engine could also just write its index directly in files on disk and when
 * executing a query, simply parse these files to get the results.
 *
 * The plug-ins are loaded and executed by the <em>Metadata Search Toolkit</em> as
 * specified in its XML configuration. The toolkit has four different
 * instantiations that each provide a different interface to the plug-ins.
 *
 * - Search engine interface
 * - Index interface
 * - Query interface
 * - Assistant interface
 *
 * The <em>search engine interface</em> is the entry point interface that a
 * plug-in must implement. Through this interface, instances of the other
 * three interfaces can be created. An instance of the <em>index interface</em>
 * is for indexing metadata and is responsible for creating an index that can
 * later be queried by an instance of the <em>query interface</em>. The
 * <em>assistant interface</em> is like an extra utility interface. One of the
 * methods that a plug-in must implement for this interface is an erase
 * method responsible for erasing the index within an given time slice.
 *
 * To build a metadata search plug-in, you need to compile a DLL that
 * implements the interface specified in the <tt>MetadataSearchPluginInterface.h</tt>
 * header file. The interface is a pure \c C interface and to avoid name
 * clashes, all names are prefixed with \c mspi. The advantage of using a pure
 * \c C interface is that you can use whatever compiler version you want to
 * create your DLL. Also you can build your plug-in in <em>debug</em> mode and
 * still have it fully working when loaded from a <em>release</em> build of
 * the <em>Metadata Search Toolkit</em>.
 *
 *
 * <br>
 * @section search_engine_interface Search engine interface
 *
 * When including the header file <tt>MetadataSearchPluginInterface.h</tt>,
 * it will automatically export the necessary symbols from your DLL to
 * make it loadable by the <em>Metadata Search Toolkit</em>. The only exported
 * function that needs to be implemented is the one for creating a search
 * engine instance.
 *
 * @code{.cpp}
 * bool mspiCreateSearchEngine(const mspiString_t configuration,
 *                             const mspiLibrary_t *library,
 *                             mspiSearchEngine_t *searchEngine)
 * @endcode
 *
 * The \c configuration parameter will be an XML document providing the settings
 * of the search engine to create. It will typically be settings about where to
 * store / how to access the index. As an example it could simply specify a path
 * to a folder that contains the index files used by the search engine. Below is
 * shown an example of how a simple search engine configuration could look like.
 *
 * @code{.xml}
 * <plugin_configuration>
 *   <storage_path>C:/IndexServer/FaceMatching</storage_path>
 * </plugin_configuration>
 * @endcode
 *
 * The root node of the configuration XML will always be <tt>pluging_configuration</tt>.
 * To parse the XML, you can use one of the utility functions provided by the
 * \c library (mspiLibrary_t). E.g. to get the storage path from the above XML,
 * you can do like shown below.
 *
 * @code{.cpp}
 * // Get storage path from configuration
 * mspiString_t storagePath;
 * if (!library->SelectSingleValue(library->Allocator, configuration, "/plugin_configuration/storage_path", NULL, &storagePath))
 *   return false;
 * @endcode
 * 
 * See mspiLibrary_t::Allocator for details about how to deal with the output values
 * (e.g. <tt>storagePath</tt> in the above example) in terms of allocation /
 * deallocation.
 *
 * The last parameter of the @ref mspiCreateSearchEngine function is \c searchEngine
 * (mspiSearchEngine_t). This represents the search engine object to create and
 * it is expected that the plug-in will initialize this. It contains five
 * function pointers which must be assigned a value. The functions must match
 * the following declarations.
 *
 * @code{.cpp}
 * void Destroy(mspiSearchEngine_t *obj)
 *
 * bool UpdateConfiguration(mspiSearchEngine_t *obj,
 *                          const mspiString_t key,
 *                          const mspiString_t value)
 *
 * bool CreateIndex(mspiSearchEngine_t *obj,
 *                  const mspiString_t deviceId,
 *                  bool objectsTracked,
 *                  const mspiString_t propertyName,
 *                  const mspiString_t configuration,
 *                  const mspiLibrary_t *library,
 *                  mspiIndex_t *index)
 *
 * bool CreateQuery(mspiSearchEngine_t *obj,
 *                  const mspiString_t deviceId,
 *                  bool objectsTracked,
 *                  const mspiString_t propertyName,
 *                  const mspiString_t configuration,
 *                  const mspiLibrary_t *library,
 *                  const mspiStringArray_t requests,
 *                  mspiQuery_t *query)
 *
 * bool CreateAssistant(mspiSearchEngine_t *obj,
 *                      const mspiString_t deviceId,
 *                      bool objectsTracked,
 *                      const mspiString_t propertyName,
 *                      const mspiString_t configuration,
 *                      const mspiLibrary_t *library,
 *                      mspiAssistant_t *assistant)
 * @endcode
 *
 * All functions take the search engine object (mspiSearchEngine_t) as the
 * first parameter. As part of the search engine object, there
 * is also an <tt>Instance</tt> member (@ref mspiInstance_t) which you e.g. can
 * assign to the underlying C++ class instance of the search engine. In this way
 * when one of the five functions above is called, it will have a direct
 * reference to the actual C++ implementation at hand.
 *
 * The mspiSearchEngine_t::Destroy function will be called when the search engine
 * is no longer needed and the resources taken should be freed.
 *
 * The mspiSearchEngine_t::UpdateConfiguration function will be called when there
 * are updates to the search engine configuration. Imagine extending the search
 * engine configuration with a setting for controlling the maximum amount of
 * memory to use for caching, like shown below.
 *
 * @code{.xml}
 * <plugin_configuration>
 *   <storage_path>C:/IndexServer/FaceMatching</storage_path>
 *   <cache_size_in_mb update_key='cache_size'>1024</cache_size_in_mb>
 * </plugin_configuration>
 * @endcode
 *
 * Here the cache size is set to 1024 mega bytes. Notice however that the
 * <tt>cache_size_in_mb</tt> node has an <tt>update_key</tt> attribute which
 * is assiged to <tt>cache_size</tt>. This tells the plug-in that if
 * mspiSearchEngine_t::UpdateConfiguration is called with the \c key parameter
 * set to <tt>cache_size</tt>, then the cache size should be changed to the size
 * specified in the \c value parameter.
 *
 * Again to extract this information from the above XML, you can use one of the
 * utility functions provided by the \c library.
 *
 * @code{.cpp}
 * // Get settings from configuration
 * const char *xpath =                 "/plugin_configuration/cache_size_in_mb";
 * const char *childXPaths[2] =        { "@update_key", NULL };
 * const char *childDefaultValues[2] = { "",            NULL };
 * mspiStringArray_t configSettings;
 * if (!library->SelectMultipleValues(library->Allocator, configuration, xpath, childXPaths, childDefaultValues, &configSettings))
 *  return false;
 * @endcode
 *
 * After successfully calling the above code, \c configSettings will contain
 * both the value of the <tt>cache_size_in_mb</tt> node and the value of the
 * <tt>update_key</tt> attribute in respectively <tt>configSettings.Strings[0]</tt>
 * and <tt>configSettings.Strings[1]</tt>.
 *
 * The mspiSearchEngine_t::CreateIndex, mspiSearchEngine_t::CreateQuery and
 * mspiSearchEngine_t::CreateAssistant functions will be described in the
 * following sections.
 *
 *
 * <br>
 * @section index_interface Index interface
 *
 * In this section we will look at how to implement the mspiSearchEngine_t::CreateIndex
 * function of the search engine. It is declared as follows.
 *
 * @code{.cpp}
 * bool CreateIndex(mspiSearchEngine_t *obj,
 *                  const mspiString_t deviceId,
 *                  bool objectsTracked,
 *                  const mspiString_t propertyName,
 *                  const mspiString_t configuration,
 *                  const mspiLibrary_t *library,
 *                  mspiIndex_t *index)
 * @endcode
 *
 * The \c deviceId parameter is a string that uniquely identifies the source
 * device from which the metadata to index is coming. When a query is made
 * later for data from this device, the same device id will be provided. It is
 * thus important that the data is indexed by the device id so that it is
 * easy / fast to retrieve again for a given device id.
 *
 * Metadata is delivered frame by frame chronologically sorted by time. Below
 * is shown an example of a metadata frame, here in the ONVIF format with a few
 * property extensions used.
 *
 * @code{.xml}
 * <MetadataStream>
 *   <VideoAnalytics>
 *    <Frame UtcTime='2018-01-18T09:41:10.015+01:00'>
 *      <Object ObjectId='10'>
 *        <Appearance>
 *          <Shape>
 *            <BoundingBox left='-0.17' top='0.068' right='-0.029' bottom='-0.19'/>
 *            <CenterOfGravity x='-0.1' y='-0.059'/>
 *          </Shape>
 *        </Appearance>
 *        <Extension>
 *          <Properties>
 *            <Property name='Age'>33</Property>
 *            <Property name='Gender'>Male</Property>
 *          </Properties>
 *        </Extension>
 *      </Object>
 *      <Extension>
 *        <Properties>
 *          <Property name="Temperature">25.4</Property>
 *        </Properties>
 *       </Extension>
 *      </Frame>
 *   </VideoAnalytics>
 * </MetadataStream>
 * @endcode
 *
 * Here the metadata frame contains one object with an object id set to
 * <tt>10</tt>. The object id is an arbitrary string that is guaranteed to be
 * unique across multiple objects within the same metadata frame. Also with the
 * object is associated a bounding box and two properties named respectively 
 * <tt>Age</tt> and <tt>Gender</tt>. The properties shown here are an extension
 * to the ONVIF standard and are basically just a list of key / value pairs
 * adding information to the given object. The <tt>Age</tt> and <tt>Gender</tt>
 * are here just examples and other properties can be present to describe other
 * information about an object. On the frame level a similar list of properties
 * is given, here with one property named <tt>Temperature</tt>. The frame
 * properties contain information that is not specific to a given object but
 * holds for the entire frame.
 *
 * The object id, in the above example, might be valid across adjacent frames.
 * So if the next metadata frame also contains an object with object id set to
 * <tt>10</tt>, then you know that this is the same object as the one in the
 * previous metadata frame. Not all metadata sources can guarantee that objects
 * are "tracked" like this across frames. However if tracked, it might enable
 * the plug-in to make a much more efficient indexing. Therefore the parameter
 * \c objectsTracked of mspiSearchEngine_t::CreateIndex will reflect whether
 * the object ids are valid across frames and the plug-in can then use this to
 * optimize the indexing.
 * 
 * The \c propertyName parameter identifies which of the properties in the
 * metadata to index. So if set to <tt>Age</tt>, it is expected that the
 * plug-in, with the above metadata frame in mind, will extract the value of
 * the property nodes with the name attribute set to <tt>Age</tt>. Since there
 * might be several objects in one metadata frame, the associated object ids
 * must also be extracted and indexed. How to easily extract the object ids and
 * property values will be described in the @ref index_object section.
 *
 * The \c configuration parameter will be an XML document providing the
 * settings of the index object to create. The configuration might be empty
 * if there are no relevant settings, but will in general contain information
 * about how to do the indexing. As an example, it can in some cases be
 * relevant to not index all data. In the metadata frame example from above
 * there was a <tt>Temperature</tt> property associated with every frame. The
 * temperature comes from a sensor and it will typically not change a lot
 * between frames. Actually indexing the temperature just once per minute
 * might be fine. The indexing plug-in however might not want to hardcode
 * this behaviour since it might differ from case to case. Instead the plug-in
 * can get this information from the \c configuration parameter. For the above
 * example it could e.g. look like this.
 *
 * @code{.xml}
 * <plugin_configuration>
 *   <msecs_to_skip>60000</msecs_to_skip>
 * </plugin_configuration>
 * @endcode
 *
 * As shown earlier, parsing the XML can be easily done using one of the utility
 * functions provided by the \c library (mspiLibrary_t) parameter.
 *
 * The last parameter of the @ref mspiSearchEngine_t::CreateIndex function is
 * \c index (mspiIndex_t) and it is expected that the plug-in will initialize
 * this. The index object is described in the next section.
 *
 *
 * <br>
 * @subsection index_object Index object
 *
 * The index object (mspiIndex_t) has the following three functions that must
 * be implemented by the plug-in.
 *
 * @code{.cpp}
 * void Destroy(mspiIndex_t *obj)
 *
 * bool UpdateConfiguration(mspiIndex_t *obj,
 *                          const mspiString_t key,
 *                          const mspiString_t value)
 *
 * bool Index(mspiIndex_t *obj,
 *            const mspiMetadata_t *data)
 * @endcode
 *
 * Each function takes as the first parameter the index object. Part of the
 * index object is an <tt>Instance</tt> member (@ref mspiInstance_t) that can be
 * initialized to the actual underlying C++ object. This gives each function
 * direct access to this underlying C++ object for doing the actual work.
 *
 * The mspiIndex_t::Destroy function will be called when the index object is
 * no longer needed and the resources taken should be freed.
 *
 * The mspiIndex_t::UpdateConfiguration function will be called when
 * there are updates to the index object configuration. If a node in the
 * configuration XML of the index object exists with an <tt>update_key</tt>
 * attribute specified, matching the \c key, then this function should update
 * the value of that configuration setting to the given \c value. As an
 * example, the index object configuration XML shown earlier can be extended
 * with an update key like shown below.
 *
 * @code{.xml}
 * <plugin_configuration>
 *   <msecs_to_skip update_key='skip_ms'>60000</msecs_to_skip>
 * </plugin_configuration>
 * @endcode
 *
 * If mspiIndex_t::UpdateConfiguration is called with \c key set to
 * <tt>skip_ms</tt>, then it should change the current milliseconds to skip
 * to that of the \c value parameter. This should of course be done
 * without destroying and re-creating the index object.
 *
 * Last comes the mspiIndex_t::Index function which is the function
 * that is responsible for doing the actual indexing. It has a \c data
 * parameter (mspiMetadata_t) which will give the implementation access to the metadata
 * to index. All metadata have a begin and end time associated with it; in the
 * data object, these can be accessed using the mspiMetadata_t::BeginTime and 
 * mspiMetadata_t::EndTime properties. Often the metadata represent a sample in
 * time and therefore the begin and end time will be the same.
 *
 * The metadata itself can be accessed using the mspiMetadata_t::GetData function
 *
 * @code{.cpp}
 * bool GetData(const mspiMetadata_t *obj,
 *              mspiByteArray_t *bytes)
 * @endcode
 *
 * With this function you will get direct access to a byte array of the raw
 * metadata and the plug-in can now from this extract the property to index.
 * The property to index was provided through the \c propertyName parameter,
 * when the index object was created (see @ref mspiSearchEngine_t::CreateIndex()).
 * 
 * If the metadata is represented in the ONVIF format using the earlier shown
 * extension scheme for properties, then these will be made available
 * automatically and there is no need to access the metadata directly. Below
 * is shown an example ONVIF metadata frame using the property extension scheme
 * (same example as shown earlier).
 *
 * @code{.xml}
 * <MetadataStream>
 *   <VideoAnalytics>
 *    <Frame UtcTime='2018-01-18T09:41:10.015+01:00'>
 *      <Object ObjectId='10'>
 *        <Appearance>
 *          <Shape>
 *            <BoundingBox left='-0.17' top='0.068' right='-0.029' bottom='-0.19'/>
 *            <CenterOfGravity x='-0.1' y='-0.059'/>
 *          </Shape>
 *        </Appearance>
 *        <Extension>
 *          <Properties>
 *            <Property name='Age'>33</Property>
 *            <Property name='Gender'>Male</Property>
 *          </Properties>
 *        </Extension>
 *      </Object>
 *      <Extension>
 *        <Properties>
 *          <Property name="Temperature">25.4</Property>
 *        </Properties>
 *       </Extension>
 *      </Frame>
 *   </VideoAnalytics>
 * </MetadataStream>
 * @endcode
 *
 * To extract the value of a frame level property, like the <tt>Temperature</tt>
 * property in the above example, you can call the function
 * mspiMetadata_t::GetProperty.
 *
 * @code{.cpp}
 * bool GetProperty(const mspiMetadata_t *obj,
 *                  const mspiString_t name,
 *                  mspiProperty_t *property)
 * @endcode
 *
 * The code to extract the value of the <tt>Temperature</tt> property, looks
 * like this.
 *
 * @code{.cpp}
 * mspiProperty_t propValue;
 * const mspiString_t propName = { 11, "Temperature" };
 * if (data->GetProperty(data, propName, &propValue) && propValue.Type == mspiStringProperty)
 * {
 *   // now index the value in propValue.String
 * }
 * @endcode
 *
 * To extract properties associated with an object, you need to use another set
 * of functions. First you can get the number of objects using the function
 * mspiMetadata_t::GetObjectCount.
 *
 * @code{.cpp}
 * int GetObjectCount(const mspiMetadata_t *obj)
 * @endcode
 *
 * Next using the function mspiMetadata_t::GetObjectProperty you can extract
 * the value of a property associated with an object.
 *
 * @code{.cpp}
 * bool GetObjectProperty(const mspiMetadata_t *obj,
 *                        const mspiString_t name,
 *                        int index,
 *                        mspiProperty_t *property);
 * @endcode
 *
 * The function works just like the mspiMetadata_t::GetProperty function
 * except that the \c index parameter now identifies which
 * object to extract the property from. The \c index parameter is a zero
 * based index and must always be lower than the object count.
 *
 * For extracting the object id that is associated with a given object, you
 * can use the function mspiMetadata_t::GetObjectId.
 *
 * @code{.cpp}
 * bool GetObjectId(const mspiMetadata_t *obj,
 *                  int index,
 *                  mspiString_t *objectId);
 * @endcode
 *
 * Again as before, the \c index parameter identifies what object to extract
 * the object id from. Putting it all together, below is shown how to extract
 * the <tt>Age</tt> property for all objects in a given metadata frame.
 *
 * @code{.cpp}
 * mspiString_t propertyName = { 3, "Age" };
 * int numberOfObjects = data->GetObjectCount(data);
 * for (int index = 0; index < numberOfObjects; index++)
 * {
 *   mspiString_t objectId;
 *   if (data->GetObjectId(data, index, &objectId))
 *   {
 *     mspiProperty_t objectProperty;
 *     if (data->GetObjectProperty(data, propertyName, index, &objectProperty))
 *     {
 *        // now index the 'Age' property value in 'objectProperty' and 'objectId' ...
 *     }
 *   }
 * }
 * @endcode
 *
 * The properties in the XML (both on object and frame level) can optionally
 * have a <tt>type</tt> attribute specified. As an example the <tt>type</tt>
 * can be <tt>ByteArray</tt> as shown below.
 * 
 * @code{.xml}
 * <Properties>
 *   <Property type="ByteArray" name="FaceTemplate">TXkgYmluYXJ5IGRhdGE=</Property>
 * </Properties>
 * @endcode
 *
 * The node value will be assumed to be a base64 encoded string and when the
 * property value is extracted using mspiMetadata_t::GetProperty or
 * mspiMetadata_t::GetObjectProperty, it will be decoded automatically and made
 * directly available as a byte array (mspiByteArray_t).
 *
 * @code{.cpp}
 * mspiProperty_t propValue;
 * const mspiString_t propName = { 12, "FaceTemplate" };
 * if (data->GetProperty(data, propName, &propValue) && propValue.Type == mspiByteArray)
 * {
 *   // now index the bytes in propValue.ByteArray
 * }
 * @endcode
 *
 * The <tt>type</tt> attribute can be set to represent any of the types in
 * @ref mspiPropertyType_t (just remove the "mspi" prefix and the "Property" suffix).
 * If no type is specified, it defaults to the string type.
 *
 *
 * <br>
 * @section query_interface Query interface
 *
 * In this section we will look at how to implement the mspiSearchEngine_t::CreateQuery
 * function of the search engine. It is declared as follows.
 *
 * @code{.cpp}
 * bool CreateQuery(mspiSearchEngine_t *obj,
 *                  const mspiString_t deviceId,
 *                  bool objectsTracked,
 *                  const mspiString_t propertyName,
 *                  const mspiString_t configuration,
 *                  const mspiLibrary_t *library,
 *                  const mspiStringArray_t requests,
 *                  mspiQuery_t *query);
 * @endcode
 *
 * The \c deviceId and \c propertyName parameters have the same meaning as for
 * the mspiSearchEngine_t::CreateIndex function. Together they identify an
 * index. Metadata that has been indexed with a given device id and property
 * name can be queried using the same device id and property name.
 *
 * The \c objectsTracked parameter is true if the objects are "tracked"
 * across frames. Here tracked means that objects with the same object id in
 * adjacent frames represent the same object.
 *
 * The \c configuration parameter will be an XML document providing the
 * settings of the query object to create. The XML describes what and how to
 * query the metadata and can easily be parsed using the utility functions
 * available in the \c library. Below is shown an example of how a query
 * configuration could look like.
 *
 * @code{.xml}
 * <plugin_configuration>
 *   <query>
 *     <match type='exact' exact_value='2'/>
 *     <match type='range' from_value='7' to_value='10'/>
 *     <match type='range' from_value='12' to_value='18'/>
 *   </query>
 * </plugin_configuration>
 * @endcode
 *
 * This will query values that are equal to 2, in the range between 7 and 10 or
 * in the range between 12 and 18. You can have as many <em>match</em> items
 * as you want in one query and they are inclusive (so or'ed together).
 *
 * The above query is an example of a <em>value query</em> and it must be supported
 * by the plug-in for queries based on properties represented by fundamental types
 * like boolean's, text, integer's, float's and enumeration's. Another class of queries
 * is the <em>similarity query</em>. Below is shown an example of how the \c configuration
 * could look for this class of queries.
 *
 * @code{.xml}
 * <plugin_configuration>
 *   <query>
 *     <match type='similar' threshold='0.12' similar_value='AAAAAAAAAAAAAIA/'/>
 *     <match type='similar' threshold='0.07' similar_value='gYAAPoiHBz/y8XE/'/>
 *   </query>
 * </plugin_configuration>
 * @endcode
 *
 * This will query values that similar to <tt>"AAAAAAAAAAAAAIA/"</tt> using a
 * threshold of 0.12 or similar to <tt>"gYAAPoiHBz/y8XE/"</tt> using a threshold
 * of 0.07. The <em>similar_value</em> is a base64 encoded string that
 * when decoded somehow describes the characteristics of what the query is
 * looking for. It could e.g. be a face template generated from a neural
 * network and the query would thus be looking for faces similar to the given
 * two. The threshold is a number between 0.0 and 1.0 can be thought upon as
 * how different the results are allowed to be in order to be included. So a
 * value close to 0.0 will mean that the results must be very similar to the
 * values specified in the query.
 *
 * The \c requests parameter is an array of strings identifying what properties
 * to include with the query results. So even though a query plug-in can
 * add a property to the query results, there is no reason to do so if it is not
 * in the \c requests array. This is only available for optimization purposes and
 * there is besides performance no reason to not just include all possible
 * properties.
 *
 * The last parameter of the @ref mspiSearchEngine_t::CreateQuery function is
 * \c query (mspiQuery_t) and it is expected that the plug-in will initialize
 * this. The query object is described in the next section.
 *
 *
 * <br>
 * @subsection query_object Query object
 *
 * The query object (mspiQuery_t) has the following five functions that must
 * be implemented by the plug-in.
 *
 * @code{.cpp}
 * void Destroy(mspiQuery_t *obj)
 *
 * bool UpdateConfiguration(mspiQuery_t *obj,
 *                          const mspiString_t key,
 *                          const mspiString_t value)
 *
 * bool Start(mspiQuery_t *obj,
 *            mspiUTCTime_t beginTime,
 *            mspiUTCTime_t endTime)
 *
 * bool Stop(mspiQuery_t *obj)
 *
 * bool GetData(mspiQuery_t *obj,
 *              mspiQueryData_t *data)
 * @endcode
 *
 * Each function takes as the first parameter the query object. Part of the
 * query object is an <tt>Instance</tt> member (@ref mspiInstance_t) that can be
 * initialized to the actual underlying C++ object. This gives each function
 * direct access to this underlying C++ object for doing the actual work.
 *
 * The mspiQuery_t::Destroy function will be called when the query object is
 * no longer needed and the resources taken should be freed.
 *
 * The mspiQuery_t::UpdateConfiguration function will be called when
 * there are updates to the query object configuration. If a node in the
 * configuration XML of the query object exists with an <tt>update_key</tt>
 * attribute specified matching the \c key, then this function should update
 * the value of that configuration setting to the given \c value.
 *
 * The plug-in must start a query when the mspiQuery_t::Start function is
 * called. The query should limit its results to be within the given
 * \c beginTime and \c endTime.
 *
 * After the query has been started, the mspiQuery_t::GetData function will be
 * called continuously until it reports that there are no more results. It is
 * expected that the plug-in, in this function, will update the \c data (mspiQueryData_t)
 * object with the next query result. To do this, the plug-in must call the
 * mspiQueryData_t::SetData function. If not called, it will be interpreted as
 * no more query results could be generated and mspiQuery_t::GetData will not be
 * called again.
 *
 * @code{.cpp}
 * bool SetData(mspiQueryData_t *obj,
 *              mspiUTCTime_t beginTime,
 *              mspiUTCTime_t endTime,
 *              bool isSequenceBegin,
 *              bool isSequenceEnd,
 *              mspiString_t objectId,
 *              const mspiByteArray_t bytes);
 * @endcode
 *
 * With the \c beginTime and \c endTime, you are specifying from when to when
 * the query is matched for this given result. So if doing a query looking for
 * when e.g. the <tt>Age</tt> property is between 40 and 50, then the time
 * stamps will represent a time slice for which this is true.
 *
 * It is a requirement, that query results generated are delivered sorted by the
 * begin time stamp. So the begin time stamp of a query result must never be
 * earlier than the begin time stamp of a previously generated query result.
 * It is okay for the begin time stamp to be equal to a previous begin time stamp
 * and also the time slices as a whole can overlap with each other in any
 * possible way as long as the begin time stamps are sorted as described above.
 * Overlapping results are often needed when combining several inclusive queries
 * in one. E.g. if you have a query that looks for faces that are similar to two
 * given face templates within some threshold, then a given face in metadata
 * might actually match both face templates.
 *
 * Clipping query results into a sequence of chunks are allowed. So if the
 * query from earlier (age between 40 and 50) is true between two time stamps,
 * it is fine to deliver the query result as two query results. If a given
 * query result is the first of a sequence, then you should set
 * \c isSequenceBegin parameter to <tt>true</tt>. If it is the last of a
 * sequence, you should set the \c isSequenceEnd parameter to <tt>true</tt>.
 * As a special case if you deliver the query result as one result, both should
 * be set to <tt>true</tt>.
 *
 * The \c objectId parameter should be set to the id of the object for which the
 * query result is matched. If the query result is not for a particular object
 * but instead holds for the entire frame, you must use the empty string.
 *
 * You can with the query result associate an array of bytes. At this point
 * this is not used for anything but could in time be used to hold more complex
 * information about the query result.
 *
 * When the query object was first created using the mspiSearchEngine_t::CreateQuery
 * function, one of the parameters was \c requests. This is an array of string
 * properties that are requested on the query results. If available, these must
 * be set on the data object as well, using the function
 *
 * @code{.cpp}
 * bool SetProperty(mspiQueryData_t *obj,
 *                  const mspiString_t name,
 *                  const mspiProperty_t property)
 * @endcode
 *
 * The last function to implement is mspiQuery_t::Stop. This will be called if
 * the query is to be stopped before all query results are delivered. When
 * called, it is expected that mspiQuery_t::GetData will be instructed to
 * return an empty query result (not calling SetData) and thereby signaling
 * that there are no more query results to get.
 *
 * When mspiQuery_t::GetData has returned an empty query result, the query is
 * stopped and a new query might be started using the mspiQuery_t::Start function.
 *
 *
 * <br>
 * @section assistant_interface Assistant interface
 *
 * In this section we will look at how to implement the mspiSearchEngine_t::CreateAssistant
 * function of the search engine. It is declared as follows.
 *
 * @code{.cpp}
 * bool CreateAssistant(mspiSearchEngine_t *obj,
 *                      const mspiString_t deviceId,
 *                      bool objectsTracked,
 *                      const mspiString_t propertyName,
 *                      const mspiString_t configuration,
 *                      const mspiLibrary_t *library,
 *                      mspiAssistant_t *assistant)
 * @endcode
 *
 * The \c deviceId, \c objectsTracked and \c propertyName parameters have the
 * same meaning as for the mspiSearchEngine_t::CreateIndex and
 * mspiSearchEngine_t::CreateQuery functions. 
 *
 * The \c configuration parameter will be an XML document providing the
 * settings of the assistant object to create. The XML can be parsed using the
 * utility functions available in the \c library.
 *
 * The last parameter of the @ref mspiSearchEngine_t::CreateAssistant function is
 * \c assistant (mspiAssistant_t) and it is expected that the plug-in will initialize
 * this. The assistant object has the following three functions that must be
 * implemented by the plug-in.
 *
 * @code{.cpp}
 * void Destroy(mspiAssistant_t *obj)
 *
 * bool Erase(mspiAssistant_t *obj,
 *            mspiUTCTime_t from,
 *            mspiUTCTime_t to)
 *
 * bool ProcessRequest(mspiAssistant_t *obj,
 *                     mspiString_t request,
 *                     mspiString_t *response)
 * @endcode
 *
 * Each function takes as the first parameter the assistant object. Part of the
 * assistant object is an <tt>Instance</tt> member (@ref mspiInstance_t) that can be
 * initialized to the actual underlying C++ object. This gives each function
 * direct access to this underlying C++ object for doing the actual work.
 *
 * The mspiAssistant_t::Destroy function will be called when the assistant object is
 * no longer needed and the resources taken should be freed.
 *
 * When metadata is erased, the index needs to be updated to reflect this so that
 * future queries will not give results about data that no longer exists. To ensure
 * this consistency between metadata and the index, the mspiAssistant_t::Erase
 * function will be called whenever metadata is erased and the plug-in can then
 * update the index accordingly.
 *
 * The last function is mspiAssistant_t::ProcessRequest and is a function that
 * in a generic way can process a request and generate a response. The request
 * is an XML document describing what to do and the response is an XML document
 * describing the result of what was done.
 *
 * Currently there is only one request that needs to be supported and only if
 * the plug-in is supporting the <em>similarity query</em> class. Such a query
 * looks like shown below where the <em>similar_value</em> is data that somehow
 * describes the characteristics of what the query is looking for
 * (see @ref query_interface for more details).
 *
 * @code{.xml}
 * <plugin_configuration>
 *   <query>
 *     <match type='similar' threshold='0.07' similar_value='gYAAPoiHBz/y8XE/'/>
 *   </query>
 * </plugin_configuration>
 * @endcode
 *
 * The data of <em>similar_value</em> could as an example represent a face
 * template and the above query would thus query faces similar to the given
 * one. The face template could come from the metadata itself but maybe you
 * want to query a face that has never been seen by the system before. This
 * is where the mspiAssistant_t::ProcessRequest function becomes relevant.
 * It is thus expected that the plug-in will be able to handle a request of
 * the following form.
 *
 * @code{.xml}
 * <request id='create_template'>
 *   <rgb_image width='320' height='200'>[Base64 encoded RGB plane]</rgb_image>
 * </request>
 * @endcode
 *
 * This is a <em>create_template</em> request and it contains an RGB image
 * from which the template should be created. Here each pixel is represented
 * by three bytes; one for red, one for green and one for blue.
 *
 * The plug-in is expected to return a response of the following form.
 *
 * @code{.xml}
 * <response>
 *   <template>[Base64 encoded template]</template>
 * </response>
 * @endcode
 *
 * The content of the <em>template</em> node represents the template generated
 * from the given image and must be created so that it can be used as a value
 * of <em>similar_value</em> when building a <em>similarity query</em>. So in
 * other words, with support for the <em>create_template</em> request, it is
 * possible to query objects that appear similar to any image provided by the
 * user.
 */


 /* === Basic types ========================================================= */

/**
 * @brief Type representing a bool.
 */
typedef bool mspiBool_t;

/**
 * @brief Type representing a 32-bit signed integer.
 */
typedef int32_t mspiInt32_t;

/**
 * @brief Type representing a 64-bit signed integer.
 */
typedef int64_t mspiInt64_t;

/**
 * @brief Type representing a double.
 */
typedef double mspiDouble_t;

/**
 * @brief Type representing a time stamp as milliseconds since January 1, 1970, 00:00:00 UTC.
 */
typedef int64_t mspiUTCTime_t;

/**
 * @brief Type representing a time span in milliseconds (can be negative).
 */
typedef int64_t mspiTimeSpan_t;

/**
 * @brief Type representing an instance (opaque type).
 */
typedef void *mspiInstance_t;

/**
 * @brief Type representing an allocator (opaque type).
 */
typedef void *mspiAllocator_t;


/* === Byte array type ===================================================== */

/**
 * @brief Type representing a byte array.
 */
struct mspiByteArray_t
{
  unsigned int Length;         ///< Number of bytes in array.
  const unsigned char *Bytes;  ///< Pointer to first byte in array.
};


/* === String type ========================================================= */

/**
 * @brief Type representing a string of characters (utf-8).
 */
struct mspiString_t
{
  unsigned int Length;     ///< Number of bytes needed to represent string; if using non ascii characters, there might be fewer characters than bytes.
  const char *Characters;  ///< Pointer to first character in string.
};

/**
 * @brief Type representing an array of strings.
 */
struct mspiStringArray_t
{
  unsigned int Length;          ///< Number of strings in array.
  const mspiString_t *Strings;  ///< Pointer to first string in array.
};


/* === Property type ======================================================= */

/**
 * @brief Type representing an enumeration of all possible property types.
 */
enum mspiPropertyType_t
{
  mspiBoolProperty,       ///< True / false boolean.
  mspiInt32Property,      ///< Signed 32-bit integer.
  mspiInt64Property,      ///< Signed 64-bit integer.
  mspiDoubleProperty,     ///< Double precision floating point.
  mspiUTCTimeProperty,    ///< UTC time stamp in milliseconds since the epoch.
  mspiByteArrayProperty,  ///< Array of bytes.
  mspiStringProperty      ///< String of characters.
};

/**
 * @brief Type representing a property.
 */
struct mspiProperty_t
{
  mspiPropertyType_t Type;      ///< What kind of property is this.
  union
  {
    mspiBool_t Bool;            ///< Boolean representation of property.
    mspiInt32_t Int32;          ///< Signed 32-bit integer representation of property.
    mspiInt64_t Int64;          ///< Signed 64-bit integer representation of property.
    mspiDouble_t Double;        ///< Double precision floating point representation of property.
    mspiUTCTime_t UCTTime;      ///< UTC time representation of property.
    mspiByteArray_t ByteArray;  ///< Byte array representation of property.
    mspiString_t String;        ///< String representation of property.
  };
};


/* === Metadata type ======================================================= */

/**
 * @brief Type representing metadata.
 */
struct mspiMetadata_t
{
  mspiInstance_t Instance;  ///< Instance data.
  mspiUTCTime_t BeginTime;  ///< Begin time stamp of metadata.
  mspiUTCTime_t EndTime;    ///< End time stamp of metadata.

  /**
   * @brief Get bytes of metadata.
   *
   * @param obj Metadata object from which to get bytes.
   * @param bytes Pointer to byte array object to update.
   * @return True if byte array was gotten successfully and false otherwise.
   */
  bool(*GetData)(const mspiMetadata_t *obj, mspiByteArray_t *bytes);

  /**
   * @brief Get property of given name.
   *
   * @param obj Metadata object from which to get property.
   * @param name Name of property to get.
   * @param property Pointer to property object to update.
   * @return True if property was gotten successfully and false otherwise.
   */
  bool(*GetProperty)(const mspiMetadata_t *obj, const mspiString_t name, mspiProperty_t *property);

  /**
   * @brief Get number of objects in metadata.
   *
   * @return Number of objects in metadata.
   */
  int(*GetObjectCount)(const mspiMetadata_t *obj);

  /**
   * @brief Get id of object.
   *
   * @param obj Metadata object from which to get object id.
   * @param index Index of object for which to get object id.
   * @param objectId Pointer to string object to update with object id.
   * @return True if object id was gotten successfully and false otherwise.
   */
  bool(*GetObjectId)(const mspiMetadata_t *obj, int index, mspiString_t *objectId);

  /**
   * @brief Get object property of given name.
   *
   * @param obj Metadata object from which to get object property.
   * @param name Name of object property to get.
   * @param index Index of object from for to get property (a number between 0 and GetObjectCount() - 1).
   * @param property Pointer to property object to update.
   * @return True if object property was gotten successfully and false otherwise.
   */
  bool(*GetObjectProperty)(const mspiMetadata_t *obj, const mspiString_t name, int index, mspiProperty_t *property);
};


/* === QueryData type ====================================================== */

/**
 * @brief Type representing query data.
 */
struct mspiQueryData_t
{
  mspiInstance_t Instance;  ///< Instance data.

  /**
   * @brief Set bytes of query data.
   *
   * @param obj Query data object for which to set bytes.
   * @param beginTime Begin time stamp of query data.
   * @param endTime End time stamp of query data.
   * @param isSequenceBegin Is this query data the first in a sequence of query results.
   * @param isSequenceEnd Is this query data the last in a sequence of query results.
   * @param objectId Id of object that this query data relates to. The empty string relates this query data to all objects.
   * @param bytes Bytes to set as data.
   * @return True if byte array was set successfully and false otherwise.
   */
  bool(*SetData)(mspiQueryData_t *obj, mspiUTCTime_t beginTime, mspiUTCTime_t endTime, bool isSequenceBegin, bool isSequenceEnd, mspiString_t objectId, const mspiByteArray_t bytes);

  /**
   * @brief Set property of given name.
   *
   * @param obj Query data object for which to set property.
   * @param name Name of property to set.
   * @param property Property to set.
   * @return True if property was set successfully and false otherwise.
   */
  bool(*SetProperty)(mspiQueryData_t *obj, const mspiString_t name, const mspiProperty_t property);
};


/* === Response type ======================================================= */

/**
 * @brief Type representing a response to a request.
 */
struct mspiResponse_t
{
  mspiInstance_t Instance;  ///< Instance data.

  /**
   * @brief Get value of response.
   *
   * @param obj Response object from which to get value.
   * @param string Pointer to string object to update.
   * @return True if response value was gotten successfully and false otherwise.
   */
  bool(*GetValue)(mspiResponse_t *obj, mspiString_t *string);

  /**
   * @brief Set value of response.
   *
   * @param obj Response object to set value of.
   * @param string String to set as value for response.
   * @return True if response value was set successfully and false otherwise.
   */
  bool(*SetValue)(mspiResponse_t *obj, const mspiString_t string);
};


/* === Library interface =================================================== */

/**
 * @brief Type representing the interface of library functions.
 */
struct mspiLibrary_t
{
  /**
   * @brief Allocator managing data allocated by library functions.
   *
   * All allocated output data from library functions are made using this
   * allocator. Whenever leaving the scope of the executing plug-in function
   * this data will be deallocated. So if you need to use the output data
   * from a library function beyond the scope of the executing plug-in function,
   * then you need to copy it.
   */
  mspiAllocator_t Allocator;

  /**
   * @brief Selects the first node in the xml document mathcing the specified xpath expression and gets the associated value.
   *
   * @param allocator Allocator of library.
   * @param xml Xml document to parse.
   * @param xpath XPath expression to evaluate.
   * @param defaultValue Default value to return if given xpath expression does not evaluate to a value. If NULL, then function will return false if xpath expression does not evaluate to a value.
   * @param result Output string to update with value of given xpath expression.
   * @return True if successful and false otherwise.
   *
   * @see Allocator
   */
  bool(*SelectSingleValue)(mspiAllocator_t allocator, const mspiString_t xml, const char *xpath, const char *defaultValue, mspiString_t *result);

  /**
   * @brief Selects the first node in the xml document mathcing each of the specified xpath expressions and gets the associated values.
   *
   * @param allocator Allocator of library.
   * @param xml Xml document to parse.
   * @param xpaths Array of xpath expressions to evaluate. This array must be NULL terminated.
   * @param defaultValues Array of default values to return if associated xpath expression does not evaluate to a value. If a default value is NULL, then this function will return false if the associated xpath expression does not evaluate to a value. This array must have exactly as many elements as there are xpath expressions.
   * @param results Output array of strings to update with values of given xpath expressions.
   * @return True if successful and false otherwise.
   *
   * @see Allocator
   */
  bool(*SelectSingleValues)(mspiAllocator_t allocator, const mspiString_t xml, const char *xpaths[], const char *defaultValues[], mspiStringArray_t *results);

  /**
   * @brief Selects all nodes matching the specified xpath and gets their values, optionally also getting values of single nodes identified by given child xpaths.
   *
   * @param allocator Allocator of library.
   * @param xml Xml document to parse.
   * @param xpath XPath expression for selecting nodes to get values from.
   * @param childXPath Array of xpath expressions for selecting child nodes to get values from. This array must be NULL terminated.
   * @param childDefaultValues Array of default values to return if associated child xpath expression does not evaluate to a value. If a default value is NULL, then this function will return false if the associated xpath expression does not evaluate to a value. This array must have exactly as many elements as there are child xpath expressions.
   * @param results Output array of strings to update with values of given xpath expressions. The number will always be a multiple of 1 + number of specified child xpaths. First comes the value of the selected node and then come the values of all selected child nodes.
   * @return True if successful and false otherwise.
   *
   * @see Allocator
   */
  bool(*SelectMultipleValues)(mspiAllocator_t allocator, const mspiString_t xml, const char *xpath, const char *childXPaths[], const char *childDefaultValues[], mspiStringArray_t *results);

  /**
   * @brief Base64 encode string.
   *
   * @param allocator Allocator of library.
   * @param stringToEncode String to encode to base64.
   * @param encodedString Output string to update with base64 encoded string.
   * @return True if successful and false otherwise.
   *
   * @see Allocator
   */
  bool(*Base64Encode)(mspiAllocator_t allocator, const mspiString_t stringToEncode, mspiString_t *encodedString);

  /**
   * @brief Base64 decode string.
   *
   * @param allocator Allocator of library.
   * @param stringToDecode Base64 encoded string to decode.
   * @param decodedString Output string to update with base64 decoded string.
   * @return True if successful and false otherwise.
   *
   * @see Allocator
   */
  bool(*Base64Decode)(mspiAllocator_t allocator, const mspiString_t stringToDecode, mspiString_t *decodedString);

  /**
   * @brief Convert time stamp to string representation in the RFC 3339 format.
   *
   * @param allocator Allocator of library.
   * @param timeStamp Time stamp to convert.
   * @param rfc3339String Output string to update with value of RFC 3339 representation.
   * @return True if successful and false otherwise.
   *
   * @see Allocator
   */
  bool(*ConvertTimeStampToRFC3339String)(mspiAllocator_t allocator, const mspiUTCTime_t timeStamp, mspiString_t *rfc3339String);

  /**
   * @brief Convert string representation in RFC 3339 format to time stamp.
   *
   * @param allocator Allocator of library.
   * @param rfc3339String RFC 3339 string representation of time stamp to convert.
   * @param timeStamp Output time stamp to update with result of conversion.
   * @return True if successful and false otherwise.
   *
   * @see Allocator
   */
  bool(*ConvertRFC3339StringToTimeStamp)(mspiAllocator_t allocator, const mspiString_t rfc3339String, mspiUTCTime_t *timeStamp);
};


/* === Index object ======================================================== */

/**
 * @brief Object used for indexing data.
 */
struct mspiIndex_t
{
  mspiInstance_t Instance;  ///< Instance data.

  /**
   * @brief Destroy index object.
   *
   * @param obj Index object to destroy.
   */
  void(*Destroy)(mspiIndex_t *obj);

  /**
   * @brief Update parameter in configuration of index object.
   *
   * It is not considered an error if the function is called with a \c key that
   * the plug-in does not know about. In this case the function should just
   * ignore the update and return true.
   *
   * @param obj Index object that will have its configuration updated.
   * @param key String identifying name of parameter to update.
   * @param value String holding value to update parameter to.
   * @return True if update was successful and false otherwise.
   */
  bool(*UpdateConfiguration)(mspiIndex_t *obj, const mspiString_t key, const mspiString_t value);

  /**
   * @brief Index data.
   *
   * @param obj Index object to use for indexing data.
   * @param data Metadata to index.
   * @return True if indexing data was successful and false otherwise.
   */
  bool(*Index)(mspiIndex_t *obj, const mspiMetadata_t *data);
};


/* === Query object ======================================================== */

/**
 * @brief Object used for querying data.
 */
struct mspiQuery_t
{
  mspiInstance_t Instance;  ///< Instance data.

  /**
   * @brief Destroy query object.
   *
   * @param obj Query object to destroy.
   */
  void(*Destroy)(mspiQuery_t *obj);

  /**
   * @brief Update parameter in configuration of query object.
   *
   * It is not considered an error if the function is called with a \c key that
   * the plug-in does not know about. In this case the function should just
   * ignore the update and return true.
   *
   * @param obj Query object that will have its configuration updated.
   * @param key String identifying name of parameter to update.
   * @param value String holding value to update parameter to.
   * @return True if update was successful and false otherwise.
   */
  bool(*UpdateConfiguration)(mspiQuery_t *obj, const mspiString_t key, const mspiString_t value);

  /**
   * @brief Start query from begin time to end time.
   *
   * When this function returns, the framework will begin to call mspiQuery_t::GetData
   * (from another thread) to get the query results. It will continue to do
   * so until it returns an empty query result.
   *
   * @param obj Query object to start.
   * @param beginTime Begin time of query.
   * @param endTime End time of query.
   * @return True if starting query was successful and false otherwise.
   */
  bool(*Start)(mspiQuery_t *obj, mspiUTCTime_t beginTime, mspiUTCTime_t endTime);

  /**
   * @brief Stop query.
   *
   * At any time during a query, this function might be called requesting it to
   * stop. As a result of calling this function it is expected that mspiQuery_t::GetData
   * will return an empty query result to signal that there are no more
   * results. Not until this has happend is the query considered stopped. Note
   * that this function will be called from another thread than mspiQuery_t::GetData and it
   * is expected that any mspiQuery_t::GetData will break any longer lasting operations
   * instantly and return an empty query result.
   *
   * @param obj Query object to start.
   * @return True if stopping query was successful and false otherwise.
   */
  bool(*Stop)(mspiQuery_t *obj);

  /**
   * @brief Get next query result.
   *
   * If mspiQueryData_t::SetData is not called on the given \c data object, it
   * will be returned empty and signal that there are no more query results
   * to get and that the query should stop.
   *
   * @param obj Query object to get query results from.
   * @param data Query data object that will be updated to hold next query result.
   * @return True if getting next query result was successful and false otherwise.
   */
  bool(*GetData)(mspiQuery_t *obj, mspiQueryData_t *data);
};


/* === Assistant object ==================================================== */

/**
 * @brief Type representing an assistant object created by the search engine.
 */
struct mspiAssistant_t
{
  mspiInstance_t Instance;  ///< Instance data.

  /**
   * @brief Destroy assistant object.
   *
   * @param obj Assistant object to destroy.
   */
  void(*Destroy)(mspiAssistant_t *obj);

  /**
   * @brief Erase index within the range of the two specified time stamps.
   *
   * @param obj Assistant object to operate on.
   * @param from Erase from this time stamp in index.
   * @param to Erase to this time stamp in index.
   * @return True if erase was successful and false otherwise.
   */
  bool(*Erase)(mspiAssistant_t *obj, mspiUTCTime_t from, mspiUTCTime_t to);

  /**
   * @brief Process generic request.
   *
   * @param obj Assistant object to operate on.
   * @param request Request to process.
   * @param response Response that will be updated to hold result of processing request.
   * @return True if processing request was successful and false otherwise.
   */
  bool(*ProcessRequest)(mspiAssistant_t *obj, const mspiString_t request, mspiResponse_t *response);
};


/* === Search engine object ================================================ */

/**
 * @brief Type representing a search engine object.
 */
struct mspiSearchEngine_t
{
  mspiInstance_t Instance;  ///< Instance data.

  /**
   * @brief Destroy search engine object.
   *
   * @param obj Search engine object to destroy.
   */
  void(*Destroy)(mspiSearchEngine_t *obj);

  /**
   * @brief Update parameter in configuration of search engine object.
   *
   * It is not considered an error if the function is called with a \c key that
   * the plug-in does not know about. In this case the function should just
   * ignore the update and return true.
   *
   * @param obj Search engine object that will have its configuration updated.
   * @param key String identifying name of parameter to update.
   * @param value String holding value to update parameter to.
   * @return True if update was successful and false otherwise.
   */
  bool(*UpdateConfiguration)(mspiSearchEngine_t *obj, const mspiString_t key, const mspiString_t value);

  /**
   * @brief Create index object.
   *
   * @param obj Search engine object to create index object from.
   * @param deviceId Id of device to which data to index belongs.
   * @param objectsTracked True if object ids are tracked between frames (an object id in one frame identifies the same object in the next frame).
   * @param propertyName Name identifying property of data to index.
   * @param configuration Configuration of index object.
   * @param library Pointer to library of utility functions that can be used in association with the index object.
   * @param index Pointer that will be updated to new instance of index object.
   * @return True if index object was created successfully and false otherwise.
   */
  bool(*CreateIndex)(mspiSearchEngine_t *obj, const mspiString_t deviceId, bool objectsTracked, const mspiString_t propertyName, const mspiString_t configuration, const mspiLibrary_t *library, mspiIndex_t *index);

  /**
   * @brief Create query object.
   *
   * @param obj Search engine object to create query object from.
   * @param deviceId Id of device to which data to query belongs.
   * @param objectsTracked True if object ids are tracked between frames (an object id in one frame identifies the same object in the next frame).
   * @param propertyName Name identifying property of data to query.
   * @param configuration Configuration of query object.
   * @param library Pointer to library of utility functions that can be used in association with the query object.
   * @param requests Strings of requested properties on query results.
   * @param query Pointer that will be updated to new instance of query object.
   * @return True if query object was created successfully and false otherwise.
   */
  bool(*CreateQuery)(mspiSearchEngine_t *obj, const mspiString_t deviceId, bool objectsTracked, const mspiString_t propertyName, const mspiString_t configuration, const mspiLibrary_t *library, const mspiStringArray_t requests, mspiQuery_t *query);

  /**
   * @brief Create assistant object.
   *
   * @param obj Search engine object to create assistant object from.
   * @param deviceId Id of device to which data to handle belongs.
   * @param objectsTracked True if object ids are tracked between frames (an object id in one frame identifies the same object in the next frame).
   * @param propertyName Name identifying property of data to handle.
   * @param configuration Configuration of assistant object.
   * @param library Pointer to library of utility functions that can be used in association with the assistant object.
   * @param assistant Pointer that will be updated to new instance of assistant object.
   * @return True if assistant object was created successfully and false otherwise.
   */
  bool(*CreateAssistant)(mspiSearchEngine_t *obj, const mspiString_t deviceId, bool objectsTracked, const mspiString_t propertyName, const mspiString_t configuration, const mspiLibrary_t *library, mspiAssistant_t *assistant);
};


/* === Plugin functions ==================================================== */

/**
 * @brief Create search engine implemented by plug-in.
 *
 * @param configuration Configuration of search engine object.
 * @param library Pointer to library of utility functions that can be used in association with the search engine object.
 * @param searchEngine Pointer that will be updated to new instance of search engine object.
 * @return True if search engine was created successfully.
 */
MSPI_API bool MSPI_FUNC(mspiCreateSearchEngine)(const mspiString_t configuration, const mspiLibrary_t *library, mspiSearchEngine_t *searchEngine);

#ifdef __cplusplus
}
#endif

#endif
