<?php
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: chain.proto

namespace Protocol\Trigger;

use Google\Protobuf\Internal\GPBType;
use Google\Protobuf\Internal\RepeatedField;
use Google\Protobuf\Internal\GPBUtil;

/**
 * Generated from protobuf message <code>protocol.Trigger.OperationTrigger</code>
 */
class OperationTrigger extends \Google\Protobuf\Internal\Message
{
    /**
     * Generated from protobuf field <code>bytes hash = 1;</code>
     */
    private $hash = '';
    /**
     * Generated from protobuf field <code>int64 index = 2;</code>
     */
    private $index = 0;

    /**
     * Constructor.
     *
     * @param array $data {
     *     Optional. Data for populating the Message object.
     *
     *     @type string $hash
     *     @type int|string $index
     * }
     */
    public function __construct($data = NULL) {
        \GPBMetadata\Chain::initOnce();
        parent::__construct($data);
    }

    /**
     * Generated from protobuf field <code>bytes hash = 1;</code>
     * @return string
     */
    public function getHash()
    {
        return $this->hash;
    }

    /**
     * Generated from protobuf field <code>bytes hash = 1;</code>
     * @param string $var
     * @return $this
     */
    public function setHash($var)
    {
        GPBUtil::checkString($var, False);
        $this->hash = $var;

        return $this;
    }

    /**
     * Generated from protobuf field <code>int64 index = 2;</code>
     * @return int|string
     */
    public function getIndex()
    {
        return $this->index;
    }

    /**
     * Generated from protobuf field <code>int64 index = 2;</code>
     * @param int|string $var
     * @return $this
     */
    public function setIndex($var)
    {
        GPBUtil::checkInt64($var);
        $this->index = $var;

        return $this;
    }

}

// Adding a class alias for backwards compatibility with the previous class name.
class_alias(OperationTrigger::class, \Protocol\Trigger_OperationTrigger::class);

