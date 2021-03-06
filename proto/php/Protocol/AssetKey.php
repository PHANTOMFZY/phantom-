<?php
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: chain.proto

namespace Protocol;

use Google\Protobuf\Internal\GPBType;
use Google\Protobuf\Internal\RepeatedField;
use Google\Protobuf\Internal\GPBUtil;

/**
 * Generated from protobuf message <code>protocol.AssetKey</code>
 */
class AssetKey extends \Google\Protobuf\Internal\Message
{
    /**
     * Generated from protobuf field <code>string issuer = 1;</code>
     */
    private $issuer = '';
    /**
     * Generated from protobuf field <code>string code = 2;</code>
     */
    private $code = '';
    /**
     *0:not limited ammount
     *
     * Generated from protobuf field <code>int32 type = 3;</code>
     */
    private $type = 0;

    /**
     * Constructor.
     *
     * @param array $data {
     *     Optional. Data for populating the Message object.
     *
     *     @type string $issuer
     *     @type string $code
     *     @type int $type
     *          0:not limited ammount
     * }
     */
    public function __construct($data = NULL) {
        \GPBMetadata\Chain::initOnce();
        parent::__construct($data);
    }

    /**
     * Generated from protobuf field <code>string issuer = 1;</code>
     * @return string
     */
    public function getIssuer()
    {
        return $this->issuer;
    }

    /**
     * Generated from protobuf field <code>string issuer = 1;</code>
     * @param string $var
     * @return $this
     */
    public function setIssuer($var)
    {
        GPBUtil::checkString($var, True);
        $this->issuer = $var;

        return $this;
    }

    /**
     * Generated from protobuf field <code>string code = 2;</code>
     * @return string
     */
    public function getCode()
    {
        return $this->code;
    }

    /**
     * Generated from protobuf field <code>string code = 2;</code>
     * @param string $var
     * @return $this
     */
    public function setCode($var)
    {
        GPBUtil::checkString($var, True);
        $this->code = $var;

        return $this;
    }

    /**
     *0:not limited ammount
     *
     * Generated from protobuf field <code>int32 type = 3;</code>
     * @return int
     */
    public function getType()
    {
        return $this->type;
    }

    /**
     *0:not limited ammount
     *
     * Generated from protobuf field <code>int32 type = 3;</code>
     * @param int $var
     * @return $this
     */
    public function setType($var)
    {
        GPBUtil::checkInt32($var);
        $this->type = $var;

        return $this;
    }

}

