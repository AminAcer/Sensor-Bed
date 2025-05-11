# ESP Entity Generator
ESP Entity Generator's goal is to generate a random initial coordinate and follow a randomized path from that starting coordinate. It will pack/publish its coordinates via OpenDIS at 20hz. This software is intended to be flashed onto a [Heltec V3 LoRa board](https://heltec.org/project/wifi-lora-32-v3/) in order to send the coordinates to a ground station via WiFi or the LoRa antenna if possible.

## Third-Party submodules changes
### ESP8266-OLED-SSD1306
  - In `esp8266-oled-ssd1306/src/OLEDDisplay.h`, add `using Print::write;` in `class OLEDDisplay` (like on line 163 above `public:`)
### OpenDIS
  - Replace the root `CMakeLists.txt` with the one below 
    - <details>
      <summary>CMakeLists.txt</summary>
      
      ```cmake
      idf_component_register(SRCS
                           "src/dis7/AcknowledgePdu.cpp"
                           "src/dis7/AcknowledgeReliablePdu.cpp"
                           "src/dis7/AcousticEmitter.cpp"
                           "src/dis7/ActionRequestPdu.cpp"
                           "src/dis7/ActionRequestReliablePdu.cpp"
                           "src/dis7/ActionResponsePdu.cpp"
                           "src/dis7/ActionResponseReliablePdu.cpp"
                           "src/dis7/AggregateIdentifier.cpp"
                           "src/dis7/AggregateMarking.cpp"
                           "src/dis7/AggregateType.cpp"
                           "src/dis7/AngleDeception.cpp"
                           "src/dis7/AngularVelocityVector.cpp"
                           "src/dis7/AntennaLocation.cpp"
                           "src/dis7/ArealObjectStatePdu.cpp"
                           "src/dis7/ArticulatedParts.cpp"
                           "src/dis7/Association.cpp"
                           "src/dis7/AttachedParts.cpp"
                           "src/dis7/Attribute.cpp"
                           "src/dis7/AttributePdu.cpp"
                           "src/dis7/BeamAntennaPattern.cpp"
                           "src/dis7/BeamData.cpp"
                           "src/dis7/BeamStatus.cpp"
                           "src/dis7/BlankingSector.cpp"
                           "src/dis7/ClockTime.cpp"
                           "src/dis7/CollisionElasticPdu.cpp"
                           "src/dis7/CollisionPdu.cpp"
                           "src/dis7/CommentPdu.cpp"
                           "src/dis7/CommentReliablePdu.cpp"
                           "src/dis7/CommunicationsNodeID.cpp"
                           "src/dis7/CreateEntityPdu.cpp"
                           "src/dis7/CreateEntityReliablePdu.cpp"
                           "src/dis7/DataPdu.cpp"
                           "src/dis7/DataQueryDatumSpecification.cpp"
                           "src/dis7/DataQueryPdu.cpp"
                           "src/dis7/DataQueryReliablePdu.cpp"
                           "src/dis7/DataReliablePdu.cpp"
                           "src/dis7/DatumSpecification.cpp"
                           "src/dis7/DeadReckoningParameters.cpp"
                           "src/dis7/DesignatorPdu.cpp"
                           "src/dis7/DetonationPdu.cpp"
                           "src/dis7/DirectedEnergyAreaAimpoint.cpp"
                           "src/dis7/DirectedEnergyDamage.cpp"
                           "src/dis7/DirectedEnergyFirePdu.cpp"
                           "src/dis7/DirectedEnergyPrecisionAimpoint.cpp"
                           "src/dis7/DirectedEnergyTargetEnergyDeposition.cpp"
                           "src/dis7/DistributedEmissionsFamilyPdu.cpp"
                           "src/dis7/EEFundamentalParameterData.cpp"
                           "src/dis7/EightByteChunk.cpp"
                           "src/dis7/ElectromagneticEmissionBeamData.cpp"
                           "src/dis7/ElectromagneticEmissionSystemData.cpp"
                           "src/dis7/ElectromagneticEmissionsPdu.cpp"
                           "src/dis7/EmitterSystem.cpp"
                           "src/dis7/EngineFuel.cpp"
                           "src/dis7/EngineFuelReload.cpp"
                           "src/dis7/EntityAssociation.cpp"
                           "src/dis7/EntityDamageStatusPdu.cpp"
                           "src/dis7/EntityID.cpp"
                           "src/dis7/EntityInformationFamilyPdu.cpp"
                           "src/dis7/EntityManagementFamilyPdu.cpp"
                           "src/dis7/EntityMarking.cpp"
                           "src/dis7/EntityStatePdu.cpp"
                           "src/dis7/EntityStateUpdatePdu.cpp"
                           "src/dis7/EntityType.cpp"
                           "src/dis7/EntityTypeVP.cpp"
                           "src/dis7/Environment.cpp"
                           "src/dis7/EnvironmentGeneral.cpp"
                           "src/dis7/EnvironmentType.cpp"
                           "src/dis7/EulerAngles.cpp"
                           "src/dis7/EventIdentifier.cpp"
                           "src/dis7/EventIdentifierLiveEntity.cpp"
                           "src/dis7/EventReportPdu.cpp"
                           "src/dis7/EventReportReliablePdu.cpp"
                           "src/dis7/Expendable.cpp"
                           "src/dis7/ExpendableDescriptor.cpp"
                           "src/dis7/ExpendableReload.cpp"
                           "src/dis7/ExplosionDescriptor.cpp"
                           "src/dis7/FalseTargetsAttribute.cpp"
                           "src/dis7/FastEntityStatePdu.cpp"
                           "src/dis7/FirePdu.cpp"
                           "src/dis7/FixedDatum.cpp"
                           "src/dis7/FourByteChunk.cpp"
                           "src/dis7/FundamentalOperationalData.cpp"
                           "src/dis7/GridAxis.cpp"
                           "src/dis7/GroupIdentifier.cpp"
                           "src/dis7/IFFFundamentalParameterData.cpp"
                           "src/dis7/IFFPdu.cpp"
                           "src/dis7/IOCommunicationsNode.cpp"
                           "src/dis7/IOEffect.cpp"
                           "src/dis7/IffDataSpecification.cpp"
                           "src/dis7/IntercomCommunicationsParameters.cpp"
                           "src/dis7/IntercomIdentifier.cpp"
                           "src/dis7/IntercomSignalPdu.cpp"
                           "src/dis7/IsPartOfPdu.cpp"
                           "src/dis7/JammingTechnique.cpp"
                           "src/dis7/LaunchedMunitionRecord.cpp"
                           "src/dis7/LayerHeader.cpp"
                           "src/dis7/LinearObjectStatePdu.cpp"
                           "src/dis7/LinearSegmentParameter.cpp"
                           "src/dis7/LiveEntityIdentifier.cpp"
                           "src/dis7/LiveEntityPdu.cpp"
                           "src/dis7/LiveSimulationAddress.cpp"
                           "src/dis7/LogisticsFamilyPdu.cpp"
                           "src/dis7/MineEntityIdentifier.cpp"
                           "src/dis7/MinefieldFamilyPdu.cpp"
                           "src/dis7/MinefieldIdentifier.cpp"
                           "src/dis7/MinefieldResponseNackPdu.cpp"
                           "src/dis7/MinefieldStatePdu.cpp"
                           "src/dis7/ModulationParameters.cpp"
                           "src/dis7/ModulationType.cpp"
                           "src/dis7/Munition.cpp"
                           "src/dis7/MunitionDescriptor.cpp"
                           "src/dis7/MunitionReload.cpp"
                           "src/dis7/NamedLocationIdentification.cpp"
                           "src/dis7/ObjectIdentifier.cpp"
                           "src/dis7/ObjectType.cpp"
                           "src/dis7/OwnershipStatus.cpp"
                           "src/dis7/Pdu.cpp"
                           "src/dis7/PduContainer.cpp"
                           "src/dis7/PduHeader.cpp"
                           "src/dis7/PduStatus.cpp"
                           "src/dis7/PduSuperclass.cpp"
                           "src/dis7/PointObjectStatePdu.cpp"
                           "src/dis7/PropulsionSystemData.cpp"
                           "src/dis7/RadioCommunicationsFamilyPdu.cpp"
                           "src/dis7/RadioIdentifier.cpp"
                           "src/dis7/RadioType.cpp"
                           "src/dis7/ReceiverPdu.cpp"
                           "src/dis7/RecordQueryReliablePdu.cpp"
                           "src/dis7/RecordQuerySpecification.cpp"
                           "src/dis7/RecordSpecification.cpp"
                           "src/dis7/RecordSpecificationElement.cpp"
                           "src/dis7/Relationship.cpp"
                           "src/dis7/RemoveEntityPdu.cpp"
                           "src/dis7/RemoveEntityReliablePdu.cpp"
                           "src/dis7/RepairCompletePdu.cpp"
                           "src/dis7/RepairResponsePdu.cpp"
                           "src/dis7/ResupplyOfferPdu.cpp"
                           "src/dis7/ResupplyReceivedPdu.cpp"
                           "src/dis7/SecondaryOperationalData.cpp"
                           "src/dis7/SeesPdu.cpp"
                           "src/dis7/Sensor.cpp"
                           "src/dis7/SeparationVP.cpp"
                           "src/dis7/ServiceRequestPdu.cpp"
                           "src/dis7/SetDataPdu.cpp"
                           "src/dis7/SetDataReliablePdu.cpp"
                           "src/dis7/SimulationAddress.cpp"
                           "src/dis7/SimulationIdentifier.cpp"
                           "src/dis7/SimulationManagementFamilyPdu.cpp"
                           "src/dis7/SimulationManagementPduHeader.cpp"
                           "src/dis7/SimulationManagementWithReliabilityFamilyPdu.cpp"
                           "src/dis7/StandardVariableSpecification.cpp"
                           "src/dis7/StartResumePdu.cpp"
                           "src/dis7/StartResumeReliablePdu.cpp"
                           "src/dis7/StopFreezePdu.cpp"
                           "src/dis7/StopFreezeReliablePdu.cpp"
                           "src/dis7/StorageFuel.cpp"
                           "src/dis7/StorageFuelReload.cpp"
                           "src/dis7/SupplyQuantity.cpp"
                           "src/dis7/SyntheticEnvironmentFamilyPdu.cpp"
                           "src/dis7/SystemIdentifier.cpp"
                           "src/dis7/TotalRecordSets.cpp"
                           "src/dis7/TrackJamData.cpp"
                           "src/dis7/TwoByteChunk.cpp"
                           "src/dis7/UAFundamentalParameter.cpp"
                           "src/dis7/UaPdu.cpp"
                           "src/dis7/UnattachedIdentifier.cpp"
                           "src/dis7/UnsignedDISInteger.cpp"
                           "src/dis7/VariableDatum.cpp"
                           "src/dis7/VariableParameter.cpp"
                           "src/dis7/VariableTransmitterParameters.cpp"
                           "src/dis7/Vector2Float.cpp"
                           "src/dis7/Vector3Double.cpp"
                           "src/dis7/Vector3Float.cpp"
                           "src/dis7/VectoringNozzleSystem.cpp"
                           "src/dis7/WarfareFamilyPdu.cpp"
                           "src/dis7/utils/ConversionUtils.cpp"
                           "src/dis7/utils/DataStream.cpp"
                         INCLUDE_DIRS "src/" "src/dis7/"
      )
      
      target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-reorder)
      ```
      </details>
  - Copy the following `opendis7_export.h` into `opendis/src/dis7/`
    - <details>
      <summary>opendis7_export.h</summary>
  
      ```c
        #ifndef OPENDIS7_EXPORT_H
        #define OPENDIS7_EXPORT_H
        
        #ifdef OPENDIS7_STATIC_DEFINE
        #  define OPENDIS7_EXPORT
        #  define OPENDIS7_NO_EXPORT
        #else
        #  ifndef OPENDIS7_EXPORT
        #    ifdef OpenDIS7_EXPORTS
                /* We are building this library */
        #      define OPENDIS7_EXPORT __attribute__((visibility("default")))
        #    else
                /* We are using this library */
        #      define OPENDIS7_EXPORT __attribute__((visibility("default")))
        #    endif
        #  endif
        
        #  ifndef OPENDIS7_NO_EXPORT
        #    define OPENDIS7_NO_EXPORT __attribute__((visibility("hidden")))
        #  endif
        #endif
        
        #ifndef OPENDIS7_DEPRECATED
        #  define OPENDIS7_DEPRECATED __attribute__ ((__deprecated__))
        #endif
        
        #ifndef OPENDIS7_DEPRECATED_EXPORT
        #  define OPENDIS7_DEPRECATED_EXPORT OPENDIS7_EXPORT OPENDIS7_DEPRECATED
        #endif
        
        #ifndef OPENDIS7_DEPRECATED_NO_EXPORT
                #  define OPENDIS7_DEPRECATED_NO_EXPORT OPENDIS7_NO_EXPORT OPENDIS7_DEPRECATED
                #endif
        
        /* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
        #if 0 /* DEFINE_NO_DEPRECATED */
        #  ifndef OPENDIS7_NO_DEPRECATED
        #    define OPENDIS7_NO_DEPRECATED
        #  endif
        #endif
        #endif /* OPENDIS7_EXPORT_H */
      ```
      </details>

## Build/Deploy to an esp32s3 board
- Make sure you're using idf.py v5.4
- `git submodule update --init --recursive`
- `export IDF_TARGET=esp32s3`
- `idf.py build`
- `idf.py -p /dev/ttyUSB0 flash monitor`
    - To exit watching the logs, use `Ctrl+]`
