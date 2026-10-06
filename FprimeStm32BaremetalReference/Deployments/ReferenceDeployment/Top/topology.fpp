module ReferenceDeployment {

  # ----------------------------------------------------------------------
  # Symbolic constants for port numbers
  # ----------------------------------------------------------------------

  enum Ports_RateGroups {
    rateGroup_1Hz
    rateGroup_0_5Hz
    rateGroup_0_25Hz
  }

  deployment topology ReferenceDeployment {

  # ----------------------------------------------------------------------
  # Subtopology imports
  # ----------------------------------------------------------------------
    import CdhCore.Subtopology
    import ComCcsds.Subtopology
    import DataProducts.Subtopology
    import FileHandling.Subtopology
    import MpuImu.Subtopology
    
  # ----------------------------------------------------------------------
  # Instances used in the topology
  # ----------------------------------------------------------------------
    instance osTime
    instance rateGroup_1Hz
    instance rateGroup_0_5Hz
    instance rateGroup_0_25Hz
    instance rateGroupDriver
    instance systemResources
    instance timer
    instance comDriver
    instance cmdSeq
    instance led
    instance gpioDriver

  # ----------------------------------------------------------------------
  # Pattern graph specifiers
  # ----------------------------------------------------------------------

    command connections instance CdhCore.cmdDisp
    event connections instance CdhCore.events
    telemetry connections instance CdhCore.tlmSend
    health connections instance CdhCore.$health
    param connections instance FileHandling.prmDb
    time connections instance osTime

  # ----------------------------------------------------------------------
  # Telemetry packets (only used when TlmPacketizer is used)
  # ----------------------------------------------------------------------

    # include "ReferenceDeploymentPackets.fppi"

  # ----------------------------------------------------------------------
  # Direct graph specifiers
  # ----------------------------------------------------------------------

    connections ComCcsds_CdhCore {
      # Core events and telemetry to communication queue
      CdhCore.events.PktSend -> ComCcsds.comQueue.comPacketQueueIn[ComCcsds.Ports_ComPacketQueue.EVENTS]
      CdhCore.tlmSend.PktSend -> ComCcsds.comQueue.comPacketQueueIn[ComCcsds.Ports_ComPacketQueue.TELEMETRY]

      # Router to Command Dispatcher
      ComCcsds.fprimeRouter.commandOut -> CdhCore.cmdDisp.seqCmdBuff
      CdhCore.cmdDisp.seqCmdStatus -> ComCcsds.fprimeRouter.cmdResponseIn
      
    }

    connections ComCcsds_FileHandling {
      # File Downlink to Communication Queue
      FileHandling.fileDownlink.bufferSendOut -> ComCcsds.comQueue.bufferQueueIn[ComCcsds.Ports_ComBufferQueue.FILE]
      ComCcsds.comQueue.bufferReturnOut[ComCcsds.Ports_ComBufferQueue.FILE] -> FileHandling.fileDownlink.bufferReturn

      # Router to File Uplink
      ComCcsds.fprimeRouter.fileOut -> FileHandling.fileUplink.bufferSendIn
      FileHandling.fileUplink.bufferSendOut -> ComCcsds.fprimeRouter.fileBufferReturnIn
    }

    connections Communications {
      # ComDriver buffer allocations
      comDriver.allocate      -> ComCcsds.commsBufferManager.bufferGetCallee
      comDriver.deallocate    -> ComCcsds.commsBufferManager.bufferSendIn
      
      # ComDriver <-> ComStub (Uplink)
      comDriver.$recv                     -> ComCcsds.comStub.drvReceiveIn
      ComCcsds.comStub.drvReceiveReturnOut -> comDriver.recvReturnIn
      
      # ComStub <-> ComDriver (Downlink)
      ComCcsds.comStub.drvSendOut      -> comDriver.$send
      comDriver.ready         -> ComCcsds.comStub.drvConnected
    }

    connections FileHandling_DataProducts {
      # Data Products to File Downlink
      DataProducts.dpCat.fileOut -> FileHandling.fileDownlink.SendFile
      FileHandling.fileDownlink.FileComplete -> DataProducts.dpCat.fileDone
    }

    connections RateGroups {
      # timer to drive rate group
      timer.CycleOut -> rateGroupDriver.CycleIn

      # 1Hz rate group
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup_1Hz] -> rateGroup_1Hz.CycleIn
      rateGroup_1Hz.RateGroupMemberOut[0] -> CdhCore.tlmSend.Run
      rateGroup_1Hz.RateGroupMemberOut[1] -> FileHandling.fileDownlink.Run
      rateGroup_1Hz.RateGroupMemberOut[2] -> systemResources.run
      rateGroup_1Hz.RateGroupMemberOut[3] -> ComCcsds.comQueue.run
      rateGroup_1Hz.RateGroupMemberOut[4] -> ComCcsds.aggregator.timeout
      rateGroup_1Hz.RateGroupMemberOut[5] -> CdhCore.cmdDisp.run
      rateGroup_1Hz.RateGroupMemberOut[6] -> comDriver.run

      # 0.5Hz rate group
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup_0_5Hz] -> rateGroup_0_5Hz.CycleIn
      rateGroup_0_5Hz.RateGroupMemberOut[0] -> cmdSeq.schedIn

      # 0.25Hz rate group
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup_0_25Hz] -> rateGroup_0_25Hz.CycleIn
      rateGroup_0_25Hz.RateGroupMemberOut[0] -> CdhCore.$health.Run
      rateGroup_0_25Hz.RateGroupMemberOut[1] -> ComCcsds.commsBufferManager.schedIn
      rateGroup_0_25Hz.RateGroupMemberOut[2] -> DataProducts.dpBufferManager.schedIn
      rateGroup_0_25Hz.RateGroupMemberOut[3] -> DataProducts.dpWriter.schedIn
      rateGroup_0_25Hz.RateGroupMemberOut[4] -> DataProducts.dpMgr.schedIn
    }

    connections CdhCore_cmdSeq {
      # Command Sequencer
      cmdSeq.comCmdOut -> CdhCore.cmdDisp.seqCmdBuff
      CdhCore.cmdDisp.seqCmdStatus -> cmdSeq.cmdResponseIn
    }

    connections ReferenceDeployment {
      # Rate Group clocked at 1Hz - output is connected to led's run input
      rateGroup_1Hz.RateGroupMemberOut[7] -> led.run
      
      # led's gpioSet output is connected to gpioDriver's gpioWrite input
      led.gpioSet -> gpioDriver.gpioWrite
    }

    connections MpuImu {
      rateGroup_1Hz.RateGroupMemberOut[8] -> MpuImu.imuManager.run
    }
  }

}
