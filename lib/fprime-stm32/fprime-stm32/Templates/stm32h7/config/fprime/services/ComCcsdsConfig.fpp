module ComCcsdsConfig {
    constant BASE_ID = 0x02000000

    module QueueSizes {
        constant comQueue    = 144
        constant aggregator  = 8
    }

    module StackSizes {
        constant comQueue   = 64 * 1024
        constant aggregator = 64 * 1024
    }

    module Priorities {
        constant aggregator = 30
        constant comQueue   = 29
    }

    module CpuAffinities {
        constant aggregator = Os.TASK_DEFAULT
        constant comQueue   = Os.TASK_DEFAULT
    }

    module QueueDepths {
        constant events = 16
        constant tlm    = 16
        constant file   = 8
    }

    module QueuePriorities {
        constant events = 0
        constant tlm    = 2
        constant file   = 1
    }

    module BuffMgr {
        constant frameAccumulatorSize = 1024
        constant commsBuffSize        = 256
        constant commsFileBuffSize    = 512
        constant commsBuffCount       = 8
        constant commsFileBuffCount   = 4
        constant commsBuffMgrId       = 200
    }
}
